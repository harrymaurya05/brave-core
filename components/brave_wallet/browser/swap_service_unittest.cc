/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <utility>

#include "base/test/bind.h"
#include "base/test/task_environment.h"
#include "brave/components/brave_wallet/browser/brave_wallet_prefs.h"
#include "brave/components/brave_wallet/browser/json_rpc_service.h"
#include "brave/components/brave_wallet/browser/swap_service.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "services/data_decoder/public/cpp/test_support/in_process_data_decoder.h"
#include "services/network/public/cpp/weak_wrapper_shared_url_loader_factory.h"
#include "services/network/test/test_url_loader_factory.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

brave_wallet::mojom::SwapParamsPtr GetCannedSwapParams() {
  auto params = brave_wallet::mojom::SwapParams::New();
  params->buy_token = "ETH";
  params->sell_token = "DAI";
  params->buy_amount = "1000000000000000000000";
  return params;
}

brave_wallet::mojom::JupiterQuoteParamsPtr GetCannedJupiterSwapParams() {
  auto params = brave_wallet::mojom::JupiterQuoteParams::New();
  params->output_mint = "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v";
  params->input_mint = "So11111111111111111111111111111111111111112";
  params->amount = "10000";
  params->slippage_percentage = 1;
  return params;
}

void OnRequestResponse(
    bool* callback_run,
    bool expected_success,
    brave_wallet::mojom::SwapResponsePtr expected_swap_response,
    const absl::optional<std::string>& expected_error,
    bool success,
    brave_wallet::mojom::SwapResponsePtr swap_response,
    const absl::optional<std::string>& error) {
  EXPECT_EQ(expected_success, success);
  EXPECT_EQ(expected_swap_response, swap_response);
  EXPECT_EQ(expected_error, error);
  *callback_run = true;
}

}  // namespace

namespace brave_wallet {

class SwapServiceUnitTest : public testing::Test {
 public:
  SwapServiceUnitTest()
      : shared_url_loader_factory_(
            base::MakeRefCounted<network::WeakWrapperSharedURLLoaderFactory>(
                &url_loader_factory_)) {
    brave_wallet::RegisterProfilePrefs(prefs_.registry());
    json_rpc_service_.reset(
        new JsonRpcService(shared_url_loader_factory_, &prefs_));
    swap_service_.reset(
        new SwapService(shared_url_loader_factory_, json_rpc_service_.get()));
  }

  ~SwapServiceUnitTest() override = default;

  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory() {
    return shared_url_loader_factory_;
  }

  void SetInterceptor(const std::string& content) {
    url_loader_factory_.SetInterceptor(base::BindLambdaForTesting(
        [&, content](const network::ResourceRequest& request) {
          url_loader_factory_.ClearResponses();
          url_loader_factory_.AddResponse(request.url.spec(), content);
        }));
  }

  void SetErrorInterceptor(const std::string& content) {
    url_loader_factory_.SetInterceptor(base::BindLambdaForTesting(
        [&](const network::ResourceRequest& request) {
          url_loader_factory_.ClearResponses();
          url_loader_factory_.AddResponse(request.url.spec(), content,
                                          net::HTTP_REQUEST_TIMEOUT);
        }));
  }

  bool IsSwapSupported(const std::string& chain_id) {
    bool result;
    base::RunLoop run_loop;
    swap_service_->IsSwapSupported(chain_id,
                                   base::BindLambdaForTesting([&](bool v) {
                                     result = v;
                                     run_loop.Quit();
                                   }));
    run_loop.Run();
    return result;
  }

 protected:
  sync_preferences::TestingPrefServiceSyncable prefs_;
  std::unique_ptr<JsonRpcService> json_rpc_service_;
  std::unique_ptr<SwapService> swap_service_;

 private:
  base::test::TaskEnvironment task_environment_;
  network::TestURLLoaderFactory url_loader_factory_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  data_decoder::test::InProcessDataDecoder in_process_data_decoder_;
};

TEST_F(SwapServiceUnitTest, GetPriceQuote) {
  SetInterceptor(R"(
    {
      "price":"1916.27547998814058355",
      "value":"0",
      "gas":"719000",
      "estimatedGas":"719000",
      "gasPrice":"26000000000",
      "protocolFee":"0",
      "minimumProtocolFee":"0",
      "buyTokenAddress":"0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
      "sellTokenAddress":"0x6b175474e89094c44da98b954eedeac495271d0f",
      "buyAmount":"1000000000000000000000",
      "sellAmount":"1916275479988140583549706",
      "sources":[],
      "allowanceTarget":"0xdef1c0ded9bec7f1a1670819833240f027b25eff",
      "sellTokenToEthRate":"1900.44962824532464391",
      "buyTokenToEthRate":"1"
    })");

  auto expected_swap_response = brave_wallet::mojom::SwapResponse::New();
  expected_swap_response->price = "1916.27547998814058355";
  expected_swap_response->value = "0";
  expected_swap_response->gas = "719000";
  expected_swap_response->estimated_gas = "719000";
  expected_swap_response->gas_price = "26000000000";
  expected_swap_response->protocol_fee = "0";
  expected_swap_response->minimum_protocol_fee = "0";
  expected_swap_response->buy_token_address =
      "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
  expected_swap_response->sell_token_address =
      "0x6b175474e89094c44da98b954eedeac495271d0f";
  expected_swap_response->buy_amount = "1000000000000000000000";
  expected_swap_response->sell_amount = "1916275479988140583549706";
  expected_swap_response->allowance_target =
      "0xdef1c0ded9bec7f1a1670819833240f027b25eff";
  expected_swap_response->sell_token_to_eth_rate = "1900.44962824532464391";
  expected_swap_response->buy_token_to_eth_rate = "1";

  bool callback_run = false;
  swap_service_->GetPriceQuote(
      GetCannedSwapParams(),
      base::BindOnce(&OnRequestResponse, &callback_run, true,
                     std::move(expected_swap_response), absl::nullopt));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetPriceQuoteError) {
  std::string error =
      R"({"code":100,"reason":"Validation Failed","validationErrors":[{"code":1000,"field":"sellAmount","reason":"should have required property 'sellAmount'"},{"code":1000,"field":"buyAmount","reason":"should have required property 'buyAmount'"},{"code":1001,"field":"","reason":"should match exactly one schema in oneOf"}]})";
  SetErrorInterceptor(error);
  bool callback_run = false;
  swap_service_->GetPriceQuote(
      brave_wallet::mojom::SwapParams::New(),
      base::BindOnce(&OnRequestResponse, &callback_run, false, nullptr, error));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetPriceQuoteUnexpectedReturn) {
  std::string error = "Could not parse response body: ";
  std::string unexpected_return = "Woot";
  SetInterceptor(unexpected_return);
  bool callback_run = false;
  swap_service_->GetPriceQuote(
      GetCannedSwapParams(),
      base::BindOnce(&OnRequestResponse, &callback_run, false, nullptr, error));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetTransactionPayload) {
  SetInterceptor(R"(
    {
      "price":"1916.27547998814058355",
      "guaranteedPrice":"1935.438234788021989386",
      "to":"0xdef1c0ded9bec7f1a1670819833240f027b25eff",
      "data":"0x0",
      "value":"0",
      "gas":"719000",
      "estimatedGas":"719000",
      "gasPrice":"26000000000",
      "protocolFee":"0",
      "minimumProtocolFee":"0",
      "buyTokenAddress":"0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
      "sellTokenAddress":"0x6b175474e89094c44da98b954eedeac495271d0f",
      "buyAmount":"1000000000000000000000",
      "sellAmount":"1916275479988140583549706",
      "sources":[],
      "allowanceTarget":"0xdef1c0ded9bec7f1a1670819833240f027b25eff",
      "sellTokenToEthRate":"1900.44962824532464391",
      "buyTokenToEthRate":"1"
    })");

  auto expected_swap_response = brave_wallet::mojom::SwapResponse::New();
  expected_swap_response->price = "1916.27547998814058355";
  expected_swap_response->guaranteed_price = "1935.438234788021989386";
  expected_swap_response->to = "0xdef1c0ded9bec7f1a1670819833240f027b25eff";
  expected_swap_response->data = "0x0";
  expected_swap_response->value = "0";
  expected_swap_response->gas = "719000";
  expected_swap_response->estimated_gas = "719000";
  expected_swap_response->gas_price = "26000000000";
  expected_swap_response->protocol_fee = "0";
  expected_swap_response->minimum_protocol_fee = "0";
  expected_swap_response->buy_token_address =
      "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
  expected_swap_response->sell_token_address =
      "0x6b175474e89094c44da98b954eedeac495271d0f";
  expected_swap_response->buy_amount = "1000000000000000000000";
  expected_swap_response->sell_amount = "1916275479988140583549706";
  expected_swap_response->allowance_target =
      "0xdef1c0ded9bec7f1a1670819833240f027b25eff";
  expected_swap_response->sell_token_to_eth_rate = "1900.44962824532464391";
  expected_swap_response->buy_token_to_eth_rate = "1";

  bool callback_run = false;
  swap_service_->GetTransactionPayload(
      GetCannedSwapParams(),
      base::BindOnce(&OnRequestResponse, &callback_run, true,
                     std::move(expected_swap_response), absl::nullopt));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetTransactionPayloadError) {
  std::string error =
      R"({"code":100,"reason":"Validation Failed","validationErrors":[{"code":1000,"field":"sellAmount","reason":"should have required property 'sellAmount'"},{"code":1000,"field":"buyAmount","reason":"should have required property 'buyAmount'"},{"code":1001,"field":"","reason":"should match exactly one schema in oneOf"}]})";
  SetErrorInterceptor(error);
  bool callback_run = false;
  swap_service_->GetTransactionPayload(
      brave_wallet::mojom::SwapParams::New(),
      base::BindOnce(&OnRequestResponse, &callback_run, false, nullptr, error));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetTransactionPayloadUnexpectedReturn) {
  std::string error = "Could not parse response body: ";
  std::string unexpected_return = "Woot";
  SetInterceptor(unexpected_return);
  bool callback_run = false;
  swap_service_->GetTransactionPayload(
      GetCannedSwapParams(),
      base::BindOnce(&OnRequestResponse, &callback_run, false, nullptr, error));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

TEST_F(SwapServiceUnitTest, GetSwapConfigurationRopsten) {
  std::string swap_api_url = "https://ropsten.api.0x.org/";
  std::string buy_token_percantage_fee = "0.00875";
  std::string fee_recipient = "0xa92D461a9a988A7f11ec285d39783A637Fdd6ba4";
  std::string affiliate_address;
  EXPECT_EQ(swap_api_url, SwapService::GetBaseSwapURL(mojom::kRopstenChainId));
  EXPECT_EQ(buy_token_percantage_fee,
            SwapService::GetFee(mojom::kRopstenChainId));
  EXPECT_EQ(fee_recipient,
            SwapService::GetFeeRecipient(mojom::kRopstenChainId));
  EXPECT_EQ(affiliate_address,
            SwapService::GetAffiliateAddress(mojom::kRopstenChainId));
}

TEST_F(SwapServiceUnitTest, GetSwapConfigurationMainnet) {
  std::string swap_api_url = "https://api.0x.org/";
  std::string buy_token_percantage_fee = "0.00875";
  std::string fee_recipient = "0xbd9420A98a7Bd6B89765e5715e169481602D9c3d";
  std::string affiliate_address = "0xbd9420A98a7Bd6B89765e5715e169481602D9c3d";
  EXPECT_EQ(swap_api_url, SwapService::GetBaseSwapURL(mojom::kMainnetChainId));
  EXPECT_EQ(buy_token_percantage_fee,
            SwapService::GetFee(mojom::kMainnetChainId));
  EXPECT_EQ(fee_recipient,
            SwapService::GetFeeRecipient(mojom::kMainnetChainId));
  EXPECT_EQ(affiliate_address,
            SwapService::GetAffiliateAddress(mojom::kMainnetChainId));
}

TEST_F(SwapServiceUnitTest, GetSwapConfigurationPolygonMainnet) {
  std::string swap_api_url = "https://polygon.api.0x.org/";
  std::string buy_token_percantage_fee = "0.00875";
  std::string fee_recipient = "0xbd9420A98a7Bd6B89765e5715e169481602D9c3d";
  std::string affiliate_address = "0xbd9420A98a7Bd6B89765e5715e169481602D9c3d";
  EXPECT_EQ(swap_api_url,
            SwapService::GetBaseSwapURL(mojom::kPolygonMainnetChainId));
  EXPECT_EQ(buy_token_percantage_fee,
            SwapService::GetFee(mojom::kPolygonMainnetChainId));
  EXPECT_EQ(fee_recipient,
            SwapService::GetFeeRecipient(mojom::kPolygonMainnetChainId));
  EXPECT_EQ(affiliate_address,
            SwapService::GetAffiliateAddress(mojom::kPolygonMainnetChainId));
}

TEST_F(SwapServiceUnitTest, GetSwapConfigurationOtherNet) {
  std::string swap_api_url;
  std::string buy_token_percantage_fee;
  std::string fee_recipient;
  std::string affiliate_address;
  EXPECT_EQ(swap_api_url, SwapService::GetBaseSwapURL(mojom::kRinkebyChainId));
  EXPECT_EQ(buy_token_percantage_fee,
            SwapService::GetFee(mojom::kRinkebyChainId));
  EXPECT_EQ(fee_recipient,
            SwapService::GetFeeRecipient(mojom::kRinkebyChainId));
  EXPECT_EQ(affiliate_address,
            SwapService::GetAffiliateAddress(mojom::kRinkebyChainId));
}

TEST_F(SwapServiceUnitTest, IsSwapSupported) {
  EXPECT_TRUE(IsSwapSupported(mojom::kMainnetChainId));
  EXPECT_TRUE(IsSwapSupported(mojom::kRopstenChainId));
  EXPECT_TRUE(IsSwapSupported(mojom::kPolygonMainnetChainId));
  EXPECT_FALSE(IsSwapSupported(mojom::kRinkebyChainId));
  EXPECT_FALSE(IsSwapSupported(""));
  EXPECT_FALSE(IsSwapSupported("invalid chain_id"));
}

TEST_F(SwapServiceUnitTest, GetJupiterQuote) {
  SetInterceptor(R"(
    {
      "data": [
        {
          "inAmount": 10000,
          "outAmount": 261273,
          "amount": 10000,
          "otherAmountThreshold": 258660,
          "outAmountWithSlippage": 258660,
          "swapMode": "ExactIn",
          "priceImpactPct": 0.008955716118219659,
          "marketInfos": [
            {
              "id": "2yNwARmTmc3NzYMETCZQjAE5GGCPgviH6hiBsxaeikTK",
              "label": "Orca",
              "inputMint": "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v",
              "outputMint": "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey",
              "notEnoughLiquidity": false,
              "inAmount": 10000,
              "outAmount": 117001203,
              "priceImpactPct": 1.196568750220778e-7,
              "lpFee": {
                "amount": 30,
                "mint": "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v",
                "pct": 0.003
              },
              "platformFee": {
                "amount": 0,
                "mint": "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey",
                "pct": 0
              }
            }
          ]
        }
      ],
      "timeTaken": 0.044471802000089156
    })");

  bool callback_run = false;
  swap_service_->GetJupiterQuote(
      GetCannedJupiterSwapParams(),
      base::BindLambdaForTesting([&](bool success,
                                     brave_wallet::mojom::JupiterQuotePtr
                                         response,
                                     const absl::optional<std::string>& error) {
        callback_run = true;
        ASSERT_TRUE(success);
        ASSERT_EQ(error, absl::nullopt);

        ASSERT_EQ(response->routes.size(), 1UL);
        EXPECT_EQ(response->routes.at(0)->in_amount, 10000ULL);
        EXPECT_EQ(response->routes.at(0)->out_amount, 261273ULL);
        EXPECT_EQ(response->routes.at(0)->amount, 10000ULL);
        EXPECT_EQ(response->routes.at(0)->other_amount_threshold, 258660ULL);
        EXPECT_EQ(response->routes.at(0)->swap_mode, "ExactIn");
        EXPECT_EQ(response->routes.at(0)->price_impact_pct,
                  0.008955716118219659);
        ASSERT_EQ(response->routes.at(0)->market_infos.size(), 1UL);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->id,
                  "2yNwARmTmc3NzYMETCZQjAE5GGCPgviH6hiBsxaeikTK");
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->label, "Orca");
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->input_mint,
                  "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v");
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->output_mint,
                  "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey");
        EXPECT_EQ(
            response->routes.at(0)->market_infos.at(0)->not_enough_liquidity,
            false);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->in_amount,
                  10000ULL);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->out_amount,
                  117001203ULL);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->price_impact_pct,
                  1.196568750220778e-7);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->lp_fee->amount,
                  30ULL);
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->lp_fee->mint,
                  "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v");
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->lp_fee->pct,
                  0.003);
        EXPECT_EQ(
            response->routes.at(0)->market_infos.at(0)->platform_fee->amount,
            0ULL);
        EXPECT_EQ(
            response->routes.at(0)->market_infos.at(0)->platform_fee->mint,
            "MNDEFzGvMt87ueuHvVU9VcTqsAP5b3fTGPsHuuPA5ey");
        EXPECT_EQ(response->routes.at(0)->market_infos.at(0)->platform_fee->pct,
                  0);
      }));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);

  // Empty JSON for conversion
  SetInterceptor(R"({})");
  callback_run = false;
  swap_service_->GetJupiterQuote(
      GetCannedJupiterSwapParams(),
      base::BindLambdaForTesting(
          [&](bool success, brave_wallet::mojom::JupiterQuotePtr response,
              const absl::optional<std::string>& error) {
            callback_run = true;
            EXPECT_FALSE(success);
            EXPECT_FALSE(response);
            EXPECT_NE(error, absl::nullopt);
          }));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);

  // Invalid JSON
  SetInterceptor(R"(foo)");
  callback_run = false;
  swap_service_->GetJupiterQuote(
      GetCannedJupiterSwapParams(),
      base::BindLambdaForTesting(
          [&](bool success, brave_wallet::mojom::JupiterQuotePtr response,
              const absl::optional<std::string>& error) {
            callback_run = true;
            EXPECT_FALSE(success);
            EXPECT_FALSE(response);
            EXPECT_NE(error, absl::nullopt);
          }));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_run);
}

}  // namespace brave_wallet
