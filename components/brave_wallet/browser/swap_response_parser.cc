/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/swap_response_parser.h"

#include <utility>
#include <vector>

#include "base/json/json_reader.h"
#include "brave/components/brave_wallet/browser/brave_wallet_constants.h"
#include "brave/components/brave_wallet/browser/json_rpc_requests_helper.h"

namespace {

absl::optional<std::string> ParseResultFromDict(
    const base::DictionaryValue* response_dict,
    const std::string& key) {
  const auto* val = response_dict->FindStringKey(key);
  if (!val) {
    return absl::nullopt;
  }

  return *val;
}

}  // namespace

namespace brave_wallet {

mojom::SwapResponsePtr ParseSwapResponse(const std::string& json,
                                         bool expect_transaction_data) {
  mojom::SwapResponse swap_response;

  // {
  //   "price":"1916.27547998814058355",
  //   "guaranteedPrice":"1935.438234788021989386",
  //   "to":"0xdef1c0ded9bec7f1a1670819833240f027b25eff",
  //   "data":"...",
  //   "value":"0",
  //   "gas":"719000",
  //   "estimatedGas":"719000",
  //   "gasPrice":"26000000000",
  //   "protocolFee":"0",
  //   "minimumProtocolFee":"0",
  //   "buyTokenAddress":"0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
  //   "sellTokenAddress":"0x6b175474e89094c44da98b954eedeac495271d0f",
  //   "buyAmount":"1000000000000000000000",
  //   "sellAmount":"1916275479988140583549706",
  //   "sources":[...],
  //   "allowanceTarget":"0xdef1c0ded9bec7f1a1670819833240f027b25eff",
  //   "sellTokenToEthRate":"1900.44962824532464391",
  //   "buyTokenToEthRate":"1"
  // }

  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                    base::JSONParserOptions::JSON_PARSE_RFC);
  auto& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return nullptr;
  }

  const base::DictionaryValue* response_dict;
  if (!records_v->GetAsDictionary(&response_dict)) {
    return nullptr;
  }

  auto price = ParseResultFromDict(response_dict, "price");
  if (!price)
    return nullptr;
  swap_response.price = *price;

  if (expect_transaction_data) {
    auto guaranteed_price =
        ParseResultFromDict(response_dict, "guaranteedPrice");
    if (!guaranteed_price)
      return nullptr;
    swap_response.guaranteed_price = *guaranteed_price;

    auto to = ParseResultFromDict(response_dict, "to");
    if (!to)
      return nullptr;
    swap_response.to = *to;

    auto data = ParseResultFromDict(response_dict, "data");
    if (!data)
      return nullptr;
    swap_response.data = *data;
  }

  auto value = ParseResultFromDict(response_dict, "value");
  if (!value)
    return nullptr;
  swap_response.value = *value;

  auto gas = ParseResultFromDict(response_dict, "gas");
  if (!gas)
    return nullptr;
  swap_response.gas = *gas;

  auto estimated_gas = ParseResultFromDict(response_dict, "estimatedGas");
  if (!estimated_gas)
    return nullptr;
  swap_response.estimated_gas = *estimated_gas;

  auto gas_price = ParseResultFromDict(response_dict, "gasPrice");
  if (!gas_price)
    return nullptr;
  swap_response.gas_price = *gas_price;

  auto protocol_fee = ParseResultFromDict(response_dict, "protocolFee");
  if (!protocol_fee)
    return nullptr;

  swap_response.protocol_fee = *protocol_fee;

  auto minimum_protocol_fee =
      ParseResultFromDict(response_dict, "minimumProtocolFee");
  if (!minimum_protocol_fee)
    return nullptr;

  swap_response.minimum_protocol_fee = *minimum_protocol_fee;

  auto buy_token_address =
      ParseResultFromDict(response_dict, "buyTokenAddress");
  if (!buy_token_address)
    return nullptr;
  swap_response.buy_token_address = *buy_token_address;

  auto sell_token_address =
      ParseResultFromDict(response_dict, "sellTokenAddress");
  if (!sell_token_address)
    return nullptr;
  swap_response.sell_token_address = *sell_token_address;

  auto buy_amount = ParseResultFromDict(response_dict, "buyAmount");
  if (!buy_amount)
    return nullptr;
  swap_response.buy_amount = *buy_amount;

  auto sell_amount = ParseResultFromDict(response_dict, "sellAmount");
  if (!sell_amount)
    return nullptr;
  swap_response.sell_amount = *sell_amount;

  auto allowance_target = ParseResultFromDict(response_dict, "allowanceTarget");
  if (!allowance_target)
    return nullptr;
  swap_response.allowance_target = *allowance_target;

  auto sell_token_to_eth_rate =
      ParseResultFromDict(response_dict, "sellTokenToEthRate");
  if (!sell_token_to_eth_rate)
    return nullptr;
  swap_response.sell_token_to_eth_rate = *sell_token_to_eth_rate;

  auto buy_token_to_eth_rate =
      ParseResultFromDict(response_dict, "buyTokenToEthRate");
  if (!buy_token_to_eth_rate)
    return nullptr;
  swap_response.buy_token_to_eth_rate = *buy_token_to_eth_rate;

  return swap_response.Clone();
}

mojom::JupiterSwapQuotePtr ParseJupiterSwapQuote(const std::string& json) {
  mojom::JupiterSwapQuote swap_quote;

  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                    base::JSONParserOptions::JSON_PARSE_RFC);

  auto& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return nullptr;
  }

  const base::DictionaryValue* response_dict = nullptr;
  if (!records_v->GetAsDictionary(&response_dict) || !response_dict)
    return nullptr;

  const base::Value* routes_value = response_dict->FindListKey("data");
  if (!routes_value)
    return nullptr;

  std::vector<mojom::JupiterRoutePtr> routes;

  for (const auto& route_value : routes_value->GetList()) {
    mojom::JupiterRoute route;

    auto in_amount = route_value.FindIntKey("inAmount");
    if (!in_amount)
      return nullptr;
    route.in_amount = *in_amount;

    auto out_amount = route_value.FindIntKey("outAmount");
    if (!out_amount)
      return nullptr;
    route.out_amount = *out_amount;

    auto amount = route_value.FindIntKey("amount");
    if (!amount)
      return nullptr;
    route.amount = *amount;

    auto other_amount_threshold =
        route_value.FindIntKey("otherAmountThreshold");
    if (!other_amount_threshold)
      return nullptr;
    route.other_amount_threshold = *other_amount_threshold;

    auto out_amount_with_slippage =
        route_value.FindIntKey("outAmountWithSlippage");
    if (!out_amount_with_slippage)
      return nullptr;
    route.out_amount_with_slippage = *out_amount_with_slippage;

    auto* swap_mode = route_value.FindStringKey("swapMode");
    if (!swap_mode)
      return nullptr;
    route.swap_mode = *swap_mode;

    auto price_impact_pct = route_value.FindDoubleKey("priceImpactPct");
    if (!price_impact_pct)
      return nullptr;
    route.price_impact_pct = *price_impact_pct;

    const base::Value* market_infos_value =
        route_value.FindListKey("marketInfos");
    if (!market_infos_value)
      return nullptr;

    for (const auto& market_info_value : market_infos_value->GetList()) {
      mojom::JupiterMarketInfo market_info;

      auto* market_info_id = market_info_value.FindStringKey("id");
      if (!market_info_id)
        return nullptr;
      market_info.id = *market_info_id;

      auto* market_info_label = market_info_value.FindStringKey("label");
      if (!market_info_label)
        return nullptr;
      market_info.label = *market_info_label;

      auto* market_info_input_mint =
          market_info_value.FindStringKey("inputMint");
      if (!market_info_input_mint)
        return nullptr;
      market_info.input_mint = *market_info_input_mint;

      auto* market_info_output_mint =
          market_info_value.FindStringKey("outputMint");
      if (!market_info_output_mint)
        return nullptr;
      market_info.output_mint = *market_info_output_mint;

      auto not_enough_liquidity =
          market_info_value.FindBoolKey("notEnoughLiquidity");
      if (!not_enough_liquidity)
        return nullptr;
      market_info.not_enough_liquidity = *not_enough_liquidity;

      auto market_info_in_amount = market_info_value.FindIntKey("inAmount");
      if (!market_info_in_amount)
        return nullptr;
      market_info.in_amount = *market_info_in_amount;

      auto market_info_out_amount = market_info_value.FindIntKey("outAmount");
      if (!market_info_out_amount)
        return nullptr;
      market_info.out_amount = *market_info_out_amount;

      auto market_info_price_impact_pct =
          market_info_value.FindDoubleKey("priceImpactPct");
      if (!market_info_price_impact_pct)
        return nullptr;
      market_info.price_impact_pct = *market_info_price_impact_pct;

      const base::Value* lp_fee_value = market_info_value.FindDictKey("lpFee");
      if (!lp_fee_value)
        return nullptr;

      mojom::JupiterFee lp_fee;
      auto lp_fee_amount = lp_fee_value->FindIntKey("amount");
      if (!lp_fee_amount)
        return nullptr;
      lp_fee.amount = *lp_fee_amount;

      auto* lp_fee_mint = lp_fee_value->FindStringKey("mint");
      if (!lp_fee_mint)
        return nullptr;
      lp_fee.mint = *lp_fee_mint;

      auto lp_fee_pct = lp_fee_value->FindDoubleKey("pct");
      if (!lp_fee_pct)
        return nullptr;
      lp_fee.pct = *lp_fee_pct;

      market_info.lp_fee = lp_fee.Clone();

      const base::Value* platform_fee_value =
          market_info_value.FindDictKey("platformFee");
      if (!platform_fee_value)
        return nullptr;

      mojom::JupiterFee platform_fee;
      auto platform_fee_amount = platform_fee_value->FindIntKey("amount");
      if (!platform_fee_amount)
        return nullptr;
      platform_fee.amount = *platform_fee_amount;

      auto* platform_fee_mint = platform_fee_value->FindStringKey("mint");
      if (!platform_fee_mint)
        return nullptr;
      platform_fee.mint = *platform_fee_mint;

      auto platform_fee_pct = platform_fee_value->FindDoubleKey("pct");
      if (!platform_fee_pct)
        return nullptr;
      platform_fee.pct = *platform_fee_pct;

      market_info.platform_fee = platform_fee.Clone();

      route.market_infos.push_back(market_info.Clone());
    }

    swap_quote.routes.push_back(route.Clone());
  }

  return swap_quote.Clone();
}

mojom::JupiterSwapTransactionsPtr ParseJupiterSwapTransactions(
    const std::string& json) {
  mojom::JupiterSwapTransactions swap_transactions;

  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                    base::JSONParserOptions::JSON_PARSE_RFC);

  auto& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return nullptr;
  }

  const base::DictionaryValue* response_dict = nullptr;
  if (!records_v->GetAsDictionary(&response_dict) || !response_dict)
    return nullptr;

  auto setup_transaction =
      ParseResultFromDict(response_dict, "setupTransaction");
  if (!setup_transaction)
    swap_transactions.setup_transaction = "";
  else
    swap_transactions.setup_transaction = *setup_transaction;

  auto swap_transaction = ParseResultFromDict(response_dict, "swapTransaction");
  if (!swap_transaction)
    return nullptr;
  swap_transactions.swap_transaction = *swap_transaction;

  auto cleanup_transaction =
      ParseResultFromDict(response_dict, "cleanupTransaction");
  if (!cleanup_transaction)
    swap_transactions.cleanup_transaction = "";
  else
    swap_transactions.cleanup_transaction = *cleanup_transaction;

  return swap_transactions.Clone();
}

std::string EncodeJupiterTransactionParams(
    mojom::JupiterTransactionParamsPtr params) {
  base::Value tx_params(base::Value::Type::DICTIONARY);
  tx_params.SetStringKey("feeAccount", brave_wallet::kSolanaFeeRecipient);
  tx_params.SetStringKey("userPublicKey", params->user_public_key);

  base::Value route(base::Value::Type::DICTIONARY);
  route.SetIntKey("inAmount", params->route->in_amount);
  route.SetIntKey("outAmount", params->route->out_amount);
  route.SetIntKey("amount", params->route->amount);
  route.SetIntKey("otherAmountThreshold",
                  params->route->other_amount_threshold);
  route.SetIntKey("outAmountWithSlippage",
                  params->route->out_amount_with_slippage);
  route.SetStringKey("swapMode", params->route->swap_mode);
  route.SetDoubleKey("priceImpactPct", params->route->price_impact_pct);

  base::Value market_infos_value(base::Value::Type::LIST);
  for (const auto& market_info : params->route->market_infos) {
    base::Value market_info_value(base::Value::Type::DICTIONARY);
    market_info_value.SetStringKey("id", market_info->id);
    market_info_value.SetStringKey("label", market_info->label);
    market_info_value.SetStringKey("inputMint", market_info->input_mint);
    market_info_value.SetStringKey("outputMint", market_info->output_mint);
    market_info_value.SetBoolKey("notEnoughLiquidity",
                                 market_info->not_enough_liquidity);
    market_info_value.SetIntKey("inAmount", market_info->in_amount);
    market_info_value.SetIntKey("outAmount", market_info->out_amount);
    market_info_value.SetDoubleKey("priceImpactPct",
                                   market_info->price_impact_pct);

    base::Value lp_fee_value(base::Value::Type::DICTIONARY);
    lp_fee_value.SetIntKey("amount", market_info->lp_fee->amount);
    lp_fee_value.SetStringKey("mint", market_info->lp_fee->mint);
    lp_fee_value.SetDoubleKey("pct", market_info->lp_fee->pct);
    market_info_value.SetKey("lpFee", std::move(lp_fee_value));

    base::Value platform_fee_value(base::Value::Type::DICTIONARY);
    platform_fee_value.SetIntKey("amount", market_info->platform_fee->amount);
    platform_fee_value.SetStringKey("mint", market_info->platform_fee->mint);
    platform_fee_value.SetDoubleKey("pct", market_info->platform_fee->pct);
    market_info_value.SetKey("platformFee", std::move(platform_fee_value));

    market_infos_value.Append(std::move(market_info_value));
  }

  route.SetKey("marketInfos", std::move(market_infos_value));
  tx_params.SetKey("route", std::move(route));

  return GetJSON(tx_params);
}

}  // namespace brave_wallet
