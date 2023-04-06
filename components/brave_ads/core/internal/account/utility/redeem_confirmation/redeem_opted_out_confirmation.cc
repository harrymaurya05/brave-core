/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/utility/redeem_confirmation/redeem_opted_out_confirmation.h"

#include <utility>

#include "base/check.h"
#include "base/functional/bind.h"
#include "brave/components/brave_ads/common/interfaces/ads.mojom.h"
#include "brave/components/brave_ads/core/internal/account/account_util.h"
#include "brave/components/brave_ads/core/internal/account/confirmations/confirmation_info.h"
#include "brave/components/brave_ads/core/internal/account/confirmations/confirmation_util.h"
#include "brave/components/brave_ads/core/internal/account/utility/redeem_confirmation/url_request_builders/create_opted_out_confirmation_url_request_builder.h"
#include "brave/components/brave_ads/core/internal/ads_client_helper.h"
#include "brave/components/brave_ads/core/internal/common/logging_util.h"
#include "brave/components/brave_ads/core/internal/common/net/http/http_status_code.h"
#include "brave/components/brave_ads/core/internal/common/url/url_request_string_util.h"
#include "brave/components/brave_ads/core/internal/common/url/url_response_string_util.h"
#include "net/http/http_status_code.h"

namespace brave_ads {

RedeemOptedOutConfirmation::~RedeemOptedOutConfirmation() = default;

// static
void RedeemOptedOutConfirmation::CreateAndRedeem(
    base::WeakPtr<RedeemConfirmationDelegate> delegate,
    const ConfirmationInfo& confirmation) {
  auto* redeem_confirmation =
      new RedeemOptedOutConfirmation(std::move(delegate));
  redeem_confirmation->Redeem(confirmation);
}

///////////////////////////////////////////////////////////////////////////////

RedeemOptedOutConfirmation::RedeemOptedOutConfirmation(
    base::WeakPtr<RedeemConfirmationDelegate> delegate) {
  DCHECK(delegate);
  delegate_ = std::move(delegate);
}

void RedeemOptedOutConfirmation::Destroy() {
  delete this;
}

void RedeemOptedOutConfirmation::Redeem(const ConfirmationInfo& confirmation) {
  DCHECK(IsValid(confirmation));
  DCHECK(!ShouldRewardUser());
  DCHECK(!confirmation.opted_in);

  BLOG(1, "Redeem opted-out confirmation");

  CreateConfirmation(confirmation);
}

void RedeemOptedOutConfirmation::CreateConfirmation(
    const ConfirmationInfo& confirmation) {
  BLOG(1, "CreateConfirmation");
  BLOG(2, "POST /v3/confirmation/{transactionId}");

  CreateOptedOutConfirmationUrlRequestBuilder url_request_builder(confirmation);
  mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();
  BLOG(6, UrlRequestToString(url_request));
  BLOG(7, UrlRequestHeadersToString(url_request));

  AdsClientHelper::GetInstance()->UrlRequest(
      std::move(url_request),
      base::BindOnce(&RedeemOptedOutConfirmation::OnCreateConfirmation,
                     base::Unretained(this), confirmation));
}

void RedeemOptedOutConfirmation::OnCreateConfirmation(
    const ConfirmationInfo& confirmation,
    const mojom::UrlResponseInfo& url_response) {
  BLOG(1, "OnCreateConfirmation");

  BLOG(6, UrlResponseToString(url_response));
  BLOG(7, UrlResponseHeadersToString(url_response));

  if (url_response.status_code != net::kHttpImATeapot) {
    const bool should_retry =
        url_response.status_code != net::HTTP_CONFLICT &&
        url_response.status_code != net::HTTP_BAD_REQUEST &&
        url_response.status_code != net::HTTP_CREATED;
    return FailedToRedeemConfirmation(confirmation, should_retry,
                                      /*should_backoff*/ should_retry);
  }

  SuccessfullyRedeemedConfirmation(confirmation);
}

void RedeemOptedOutConfirmation::SuccessfullyRedeemedConfirmation(
    const ConfirmationInfo& confirmation) {
  BLOG(1, "Successfully redeemed opted-out "
              << confirmation.type << " confirmation for "
              << confirmation.ad_type << " with transaction id "
              << confirmation.transaction_id << " and creative instance id "
              << confirmation.creative_instance_id);

  if (delegate_) {
    delegate_->OnDidRedeemOptedOutConfirmation(confirmation);
  }

  Destroy();
}

void RedeemOptedOutConfirmation::FailedToRedeemConfirmation(
    const ConfirmationInfo& confirmation,
    const bool should_retry,
    bool should_backoff) {
  BLOG(1, "Failed to redeem opted-out "
              << confirmation.type << " confirmation for "
              << confirmation.ad_type << " with transaction id "
              << confirmation.transaction_id << " and creative instance id "
              << confirmation.creative_instance_id);

  if (delegate_) {
    delegate_->OnFailedToRedeemConfirmation(confirmation, should_retry,
                                            should_backoff);
  }

  Destroy();
}

}  // namespace brave_ads
