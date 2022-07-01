/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/swap_response_parser.h"

#include <utility>
#include <vector>

#include "base/json/json_reader.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "brave/components/brave_wallet/browser/brave_wallet_constants.h"
#include "brave/components/brave_wallet/browser/json_rpc_requests_helper.h"
#include "brave/components/brave_wallet/browser/json_rpc_response_parser.h"
#include "brave/components/json/rs/src/lib.rs.h"

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

absl::optional<uint64_t> ParseUint64ResultFromStringDictValue(
    const base::Value& dict_value,
    const std::string& key) {
  auto* value = dict_value.FindStringKey(key);
  if (!value)
    return absl::nullopt;

  uint64_t ret;
  if (base::StringToUint64(*value, &ret))
    return ret;

  return absl::nullopt;
}

// Parses uint64_t value from a JSON integer field.
//
// Returns empty result for values that are either negative or beyond maximum
// safe integer in Javascript (ES6 section 20.1.2.6 Number.MAX_SAFE_INTEGER).
//
// For larger values, use ParseUint64ResultFromStringDictValue after converting
// them to strings.
absl::optional<uint64_t> ParseUint64ResultFromIntegerDictValue(
    const base::Value& dict_value,
    const std::string& key) {
  auto value = dict_value.FindDoubleKey(key);
  if (!value)
    return absl::nullopt;

  if (*value < 0 ||
      *value > static_cast<double>(brave_wallet::kMaxSafeIntegerUint64))
    return absl::nullopt;

  return static_cast<uint64_t>(*value);
}

absl::optional<const base::Value::List> GetRoutesFromJupiterSwapQuote(
    const std::string& json) {
  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                    base::JSONParserOptions::JSON_PARSE_RFC);
  auto& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return absl::nullopt;
  }

  const base::DictionaryValue* response_dict;
  if (!records_v->GetAsDictionary(&response_dict))
    return absl::nullopt;

  const base::Value* routes_value = response_dict->FindListKey("data");
  if (!routes_value)
    return absl::nullopt;

  return routes_value->GetList().Clone();
}

}  // namespace

namespace brave_wallet {

mojom::SwapResponsePtr ParseSwapResponse(const std::string& json,
                                         bool expect_transaction_data) {
  auto swap_response = mojom::SwapResponse::New();

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
  if (!records_v->GetAsDictionary(&response_dict))
    return nullptr;

  auto price = ParseResultFromDict(response_dict, "price");
  if (!price)
    return nullptr;
  swap_response->price = *price;

  if (expect_transaction_data) {
    auto guaranteed_price =
        ParseResultFromDict(response_dict, "guaranteedPrice");
    if (!guaranteed_price)
      return nullptr;
    swap_response->guaranteed_price = *guaranteed_price;

    auto to = ParseResultFromDict(response_dict, "to");
    if (!to)
      return nullptr;
    swap_response->to = *to;

    auto data = ParseResultFromDict(response_dict, "data");
    if (!data)
      return nullptr;
    swap_response->data = *data;
  }

  auto value = ParseResultFromDict(response_dict, "value");
  if (!value)
    return nullptr;
  swap_response->value = *value;

  auto gas = ParseResultFromDict(response_dict, "gas");
  if (!gas)
    return nullptr;
  swap_response->gas = *gas;

  auto estimated_gas = ParseResultFromDict(response_dict, "estimatedGas");
  if (!estimated_gas)
    return nullptr;
  swap_response->estimated_gas = *estimated_gas;

  auto gas_price = ParseResultFromDict(response_dict, "gasPrice");
  if (!gas_price)
    return nullptr;
  swap_response->gas_price = *gas_price;

  auto protocol_fee = ParseResultFromDict(response_dict, "protocolFee");
  if (!protocol_fee)
    return nullptr;

  swap_response->protocol_fee = *protocol_fee;

  auto minimum_protocol_fee =
      ParseResultFromDict(response_dict, "minimumProtocolFee");
  if (!minimum_protocol_fee)
    return nullptr;

  swap_response->minimum_protocol_fee = *minimum_protocol_fee;

  auto buy_token_address =
      ParseResultFromDict(response_dict, "buyTokenAddress");
  if (!buy_token_address)
    return nullptr;
  swap_response->buy_token_address = *buy_token_address;

  auto sell_token_address =
      ParseResultFromDict(response_dict, "sellTokenAddress");
  if (!sell_token_address)
    return nullptr;
  swap_response->sell_token_address = *sell_token_address;

  auto buy_amount = ParseResultFromDict(response_dict, "buyAmount");
  if (!buy_amount)
    return nullptr;
  swap_response->buy_amount = *buy_amount;

  auto sell_amount = ParseResultFromDict(response_dict, "sellAmount");
  if (!sell_amount)
    return nullptr;
  swap_response->sell_amount = *sell_amount;

  auto allowance_target = ParseResultFromDict(response_dict, "allowanceTarget");
  if (!allowance_target)
    return nullptr;
  swap_response->allowance_target = *allowance_target;

  auto sell_token_to_eth_rate =
      ParseResultFromDict(response_dict, "sellTokenToEthRate");
  if (!sell_token_to_eth_rate)
    return nullptr;
  swap_response->sell_token_to_eth_rate = *sell_token_to_eth_rate;

  auto buy_token_to_eth_rate =
      ParseResultFromDict(response_dict, "buyTokenToEthRate");
  if (!buy_token_to_eth_rate)
    return nullptr;
  swap_response->buy_token_to_eth_rate = *buy_token_to_eth_rate;

  return swap_response;
}

mojom::JupiterQuotePtr ParseJupiterQuote(const std::string& json) {
  //  {
  //    "data": [
  //      {
  //        "inAmount": 10000,
  //        "outAmount": 261273,
  //        "amount": 10000,
  //        "otherAmountThreshold": 258660,
  //        "outAmountWithSlippage": 258660,
  //        "swapMode": "ExactIn",
  //        "priceImpactPct": 0.008955716118219659,
  //        "marketInfos": [
  //          {
  //            "id": "2yNwARmTmc3NzYMETCZQjAE5GGCPgviH6hiBsxaeikTK",
  //            "label": "Orca",
  //            "inputMint": "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v",
  //            "outputMint": "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey",
  //            "notEnoughLiquidity": false,
  //            "inAmount": 10000,
  //            "outAmount": 117001203,
  //            "priceImpactPct": 1.196568750220778e-7,
  //            "lpFee": {
  //              "amount": 30,
  //              "mint": "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v",
  //              "pct": 0.003
  //            },
  //            "platformFee": {
  //              "amount": 0,
  //              "mint": "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey",
  //              "pct": 0
  //            }
  //          }
  //        ]
  //      }
  //    ],
  //    "timeTaken": 0.044471802000089156
  //  }

  // STEP 1: convert JSON integer fields in routes to string values.
  auto converted_json = ConvertMultiUint64InObjectArrayToString(
      "/data", {"inAmount", "outAmount", "amount", "otherAmountThreshold"},
      json);
  if (!converted_json) {
    LOG(ERROR) << "Failed to parse integer fields, JSON is: " << json;
    return nullptr;
  }

  // STEP 2: Obtain number of routes in swap quote.
  auto routes_value_1 = GetRoutesFromJupiterSwapQuote(*converted_json);
  if (!routes_value_1)
    return nullptr;

  // STEP 3: Loop over the routes and convert JSON integer fields in
  // marketInfos to string values.
  for (int i = 0; i < static_cast<int>(routes_value_1->size()); i++) {
    converted_json = ConvertMultiUint64InObjectArrayToString(
        base::StringPrintf("/data/%d/marketInfos", i),
        {"inAmount", "outAmount"}, *converted_json);
  }

  // STEP 4: Parse the final converted JSON again to obtain the routes.
  auto routes_value = GetRoutesFromJupiterSwapQuote(*converted_json);
  if (!routes_value)
    return nullptr;

  auto swap_quote = mojom::JupiterQuote::New();
  std::vector<mojom::JupiterRoutePtr> routes;

  // STEP 5: Parse individual fields to populate mojom::JupiterSwapQuote
  for (const auto& route_value : *routes_value) {
    mojom::JupiterRoute route;

    auto in_amount =
        ParseUint64ResultFromStringDictValue(route_value, "inAmount");
    if (!in_amount)
      return nullptr;
    route.in_amount = *in_amount;

    auto out_amount =
        ParseUint64ResultFromStringDictValue(route_value, "outAmount");
    if (!out_amount)
      return nullptr;
    route.out_amount = *out_amount;

    auto amount = ParseUint64ResultFromStringDictValue(route_value, "amount");
    if (!amount)
      return nullptr;
    route.amount = *amount;

    auto other_amount_threshold = ParseUint64ResultFromStringDictValue(
        route_value, "otherAmountThreshold");
    if (!other_amount_threshold)
      return nullptr;
    route.other_amount_threshold = *other_amount_threshold;

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

      auto market_info_in_amount =
          ParseUint64ResultFromStringDictValue(market_info_value, "inAmount");
      if (!market_info_in_amount)
        return nullptr;
      market_info.in_amount = *market_info_in_amount;

      auto market_info_out_amount =
          ParseUint64ResultFromStringDictValue(market_info_value, "outAmount");
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

      // Parse lpFee->amount field as a JSON integer field, since the
      // values are typically very small, and intermediate conversion to string
      // is expensive due to its deep nesting.
      mojom::JupiterFee lp_fee;
      auto lp_fee_amount =
          ParseUint64ResultFromIntegerDictValue(*lp_fee_value, "amount");
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

      // Parse platformFee->amount field as a JSON integer field, since the
      // values are typically very small, and intermediate conversion to string
      // is expensive due to its deep nesting.
      mojom::JupiterFee platform_fee;
      auto platform_fee_amount =
          ParseUint64ResultFromIntegerDictValue(*platform_fee_value, "amount");
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

    swap_quote->routes.push_back(route.Clone());
  }

  return swap_quote;
}

mojom::JupiterSwapTransactionsPtr ParseJupiterSwapTransactions(
    const std::string& json) {
  auto swap_transactions = mojom::JupiterSwapTransactions::New();

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
    swap_transactions->setup_transaction = "";
  else
    swap_transactions->setup_transaction = *setup_transaction;

  auto swap_transaction = ParseResultFromDict(response_dict, "swapTransaction");
  if (!swap_transaction)
    return nullptr;
  swap_transactions->swap_transaction = *swap_transaction;

  auto cleanup_transaction =
      ParseResultFromDict(response_dict, "cleanupTransaction");
  if (!cleanup_transaction)
    swap_transactions->cleanup_transaction = "";
  else
    swap_transactions->cleanup_transaction = *cleanup_transaction;

  return swap_transactions;
}

std::string EncodeJupiterTransactionParams(mojom::JupiterSwapParamsPtr params) {
  base::Value tx_params(base::Value::Type::DICTIONARY);
  tx_params.SetStringKey("feeAccount", brave_wallet::kSolanaFeeRecipient);
  tx_params.SetStringKey("userPublicKey", params->user_public_key);

  base::Value route(base::Value::Type::DICTIONARY);
  route.SetStringKey("inAmount",
                     base::NumberToString(params->route->in_amount));
  route.SetStringKey("outAmount",
                     base::NumberToString(params->route->out_amount));
  route.SetStringKey("amount", base::NumberToString(params->route->amount));
  route.SetStringKey(
      "otherAmountThreshold",
      base::NumberToString(params->route->other_amount_threshold));
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
    market_info_value.SetStringKey(
        "inAmount", base::NumberToString(market_info->in_amount));
    market_info_value.SetStringKey(
        "outAmount", base::NumberToString(market_info->out_amount));
    market_info_value.SetDoubleKey("priceImpactPct",
                                   market_info->price_impact_pct);

    base::Value lp_fee_value(base::Value::Type::DICTIONARY);
    lp_fee_value.SetStringKey(
        "amount", base::NumberToString(market_info->lp_fee->amount));
    lp_fee_value.SetStringKey("mint", market_info->lp_fee->mint);
    lp_fee_value.SetDoubleKey("pct", market_info->lp_fee->pct);

    market_info_value.SetKey("lpFee", std::move(lp_fee_value));

    base::Value platform_fee_value(base::Value::Type::DICTIONARY);
    platform_fee_value.SetStringKey(
        "amount", base::NumberToString(market_info->platform_fee->amount));
    platform_fee_value.SetStringKey("mint", market_info->platform_fee->mint);
    platform_fee_value.SetDoubleKey("pct", market_info->platform_fee->pct);
    market_info_value.SetKey("platformFee", std::move(platform_fee_value));

    market_infos_value.Append(std::move(market_info_value));
  }

  route.SetKey("marketInfos", std::move(market_infos_value));
  tx_params.SetKey("route", std::move(route));

  std::string result = GetJSON(tx_params);
  result = std::string(
      json::convert_string_value_to_uint64("/route/inAmount", result, false));
  result = std::string(
      json::convert_string_value_to_uint64("/route/outAmount", result, false));
  result = std::string(
      json::convert_string_value_to_uint64("/route/amount", result, false));
  result = std::string(json::convert_string_value_to_uint64(
      "/route/otherAmountThreshold", result, false));

  for (int i = 0; i < static_cast<int>(params->route->market_infos.size());
       i++) {
    result = std::string(json::convert_string_value_to_uint64(
        base::StringPrintf("/route/marketInfos/%d/inAmount", i), result,
        false));
    result = std::string(json::convert_string_value_to_uint64(
        base::StringPrintf("/route/marketInfos/%d/outAmount", i), result,
        false));
    result = std::string(json::convert_string_value_to_uint64(
        base::StringPrintf("/route/marketInfos/%d/lpFee/amount", i), result,
        false));
    result = std::string(json::convert_string_value_to_uint64(
        base::StringPrintf("/route/marketInfos/%d/platformFee/amount", i),
        result, false));
  }

  return result;
}

}  // namespace brave_wallet
