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

bool ParseResultFromDict(const base::Value::Dict& response_dict,
                         const std::string& key,
                         std::string* output_val) {
  const auto* val = response_dict.FindString(key);
  if (!val) {
    return false;
  }
  *output_val = *val;
  return true;
}

bool ParseUint64ResultFromValue(const base::Value& value,
                                const std::string& key,
                                uint64_t* output_val) {
  auto val = value.FindIntKey(key);
  if (!val)
    return false;

  *output_val = static_cast<uint64_t>(*val);
  return true;
}

bool ParseDoubleResultFromValue(const base::Value& value,
                                const std::string& key,
                                double* output_val) {
  auto val = value.FindDoubleKey(key);
  if (!val)
    return false;

  *output_val = *val;
  return true;
}

bool ParseStringResultFromValue(const base::Value& value,
                                const std::string& key,
                                std::string* output_val) {
  auto* val = value.FindStringKey(key);
  if (!val)
    return false;

  *output_val = *val;
  return true;
}

bool ParseBoolResultFromValue(const base::Value& value,
                              const std::string& key,
                              bool* output_val) {
  auto val = value.FindBoolKey(key);
  if (!val)
    return false;

  *output_val = *val;
  return true;
}

}  // namespace

namespace brave_wallet {

bool ParseSwapResponse(const std::string& json,
                       bool expect_transaction_data,
                       mojom::SwapResponsePtr* swap_response) {
  DCHECK(swap_response);
  *swap_response = mojom::SwapResponse::New();
  auto& response = *swap_response;

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
  if (!records_v || !records_v->is_dict()) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return false;
  }

  const auto& response_dict = records_v->GetDict();
  if (!ParseResultFromDict(response_dict, "price", &response->price))
    return false;

  if (expect_transaction_data) {
    if (!ParseResultFromDict(response_dict, "guaranteedPrice",
                             &response->guaranteed_price))
      return false;

    if (!ParseResultFromDict(response_dict, "to", &response->to))
      return false;

    if (!ParseResultFromDict(response_dict, "data", &response->data))
      return false;
  }

  if (!ParseResultFromDict(response_dict, "value", &response->value))
    return false;

  if (!ParseResultFromDict(response_dict, "gas", &response->gas))
    return false;

  if (!ParseResultFromDict(response_dict, "estimatedGas",
                           &response->estimated_gas))
    return false;

  if (!ParseResultFromDict(response_dict, "gasPrice", &response->gas_price))
    return false;

  if (!ParseResultFromDict(response_dict, "protocolFee",
                           &response->protocol_fee))
    return false;

  if (!ParseResultFromDict(response_dict, "minimumProtocolFee",
                           &response->minimum_protocol_fee))
    return false;

  if (!ParseResultFromDict(response_dict, "buyTokenAddress",
                           &response->buy_token_address))
    return false;

  if (!ParseResultFromDict(response_dict, "sellTokenAddress",
                           &response->sell_token_address))
    return false;

  if (!ParseResultFromDict(response_dict, "buyAmount", &response->buy_amount))
    return false;

  if (!ParseResultFromDict(response_dict, "sellAmount", &response->sell_amount))
    return false;

  if (!ParseResultFromDict(response_dict, "allowanceTarget",
                           &response->allowance_target))
    return false;

  if (!ParseResultFromDict(response_dict, "sellTokenToEthRate",
                           &response->sell_token_to_eth_rate))
    return false;

  if (!ParseResultFromDict(response_dict, "buyTokenToEthRate",
                           &response->buy_token_to_eth_rate))
    return false;

  return true;
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

    if (!ParseUint64ResultFromValue(route_value, "inAmount", &route.in_amount))
      return nullptr;

    if (!ParseUint64ResultFromValue(route_value, "outAmount",
                                    &route.out_amount))
      return nullptr;

    if (!ParseUint64ResultFromValue(route_value, "amount", &route.amount))
      return nullptr;

    if (!ParseUint64ResultFromValue(route_value, "otherAmountThreshold",
                                    &route.other_amount_threshold))
      return nullptr;

    if (!ParseUint64ResultFromValue(route_value, "outAmountWithSlippage",
                                    &route.out_amount_with_slippage))
      return nullptr;

    if (!ParseStringResultFromValue(route_value, "swapMode", &route.swap_mode))
      return nullptr;

    if (!ParseDoubleResultFromValue(route_value, "priceImpactPct",
                                    &route.price_impact_pct))
      return nullptr;

    const base::Value* market_infos_value =
        route_value.FindListKey("marketInfos");
    if (!market_infos_value)
      return nullptr;

    for (const auto& market_info_value : market_infos_value->GetList()) {
      mojom::JupiterMarketInfo market_info;

      if (!ParseStringResultFromValue(market_info_value, "id", &market_info.id))
        return nullptr;

      if (!ParseStringResultFromValue(market_info_value, "label",
                                      &market_info.label))
        return nullptr;

      if (!ParseStringResultFromValue(market_info_value, "inputMint",
                                      &market_info.input_mint))
        return nullptr;

      if (!ParseStringResultFromValue(market_info_value, "outputMint",
                                      &market_info.output_mint))
        return nullptr;

      if (!ParseBoolResultFromValue(market_info_value, "notEnoughLiquidity",
                                    &market_info.not_enough_liquidity))
        return nullptr;

      if (!ParseUint64ResultFromValue(market_info_value, "inAmount",
                                      &market_info.in_amount))
        return nullptr;

      if (!ParseUint64ResultFromValue(market_info_value, "outAmount",
                                      &market_info.out_amount))
        return nullptr;

      if (!ParseDoubleResultFromValue(market_info_value, "priceImpactPct",
                                      &market_info.price_impact_pct))
        return nullptr;

      const base::Value* lp_fee_value = market_info_value.FindDictKey("lpFee");
      if (!lp_fee_value)
        return nullptr;

      mojom::JupiterFee lp_fee;
      if (!ParseUint64ResultFromValue(*lp_fee_value, "amount", &lp_fee.amount))
        return nullptr;

      if (!ParseStringResultFromValue(*lp_fee_value, "mint", &lp_fee.mint))
        return nullptr;

      if (!ParseDoubleResultFromValue(*lp_fee_value, "pct", &lp_fee.pct))
        return nullptr;

      market_info.lp_fee = lp_fee.Clone();

      const base::Value* platform_fee_value =
          market_info_value.FindDictKey("platformFee");
      if (!platform_fee_value)
        return nullptr;

      mojom::JupiterFee platform_fee;
      if (!ParseUint64ResultFromValue(*platform_fee_value, "amount",
                                      &platform_fee.amount))
        return nullptr;

      if (!ParseStringResultFromValue(*platform_fee_value, "mint",
                                      &platform_fee.mint))
        return nullptr;

      if (!ParseDoubleResultFromValue(*platform_fee_value, "pct",
                                      &platform_fee.pct))
        return nullptr;

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

  if (!ParseResultFromDict(response_dict, "setupTransaction",
                           &swap_transactions.setup_transaction))
    return nullptr;

  if (!ParseResultFromDict(response_dict, "swapTransaction",
                           &swap_transactions.swap_transaction))
    return nullptr;

  if (!ParseResultFromDict(response_dict, "cleanupTransaction",
                           &swap_transactions.cleanup_transaction))
    return nullptr;

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
