/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_vpn/brave_vpn_navigation_throttle.h"

#include <memory>
#include <utility>

#include "brave/components/brave_vpn/brave_vpn_service.h"
#include "content/public/browser/navigation_handle.h"
#include "net/base/net_errors.h"

namespace brave_vpn {

// static
std::unique_ptr<BraveVpnNavigationThrottle>
BraveVpnNavigationThrottle::MaybeCreateThrottleFor(
    content::NavigationHandle* navigation_handle,
    BraveVpnService* brave_vpn_service) {
  LOG(ERROR) << "BSC]] BraveVpnNavigationThrottle::MaybeCreateThrottleFor";
  if (!brave_vpn_service) {
    LOG(ERROR)
        << "BSC]] WHOOPS - you might need to enable brave://flags/#brave-vpn";
    return nullptr;
  }
  return std::make_unique<BraveVpnNavigationThrottle>(navigation_handle,
                                                      brave_vpn_service);
}

BraveVpnNavigationThrottle::BraveVpnNavigationThrottle(
    content::NavigationHandle* navigation_handle,
    BraveVpnService* brave_vpn_service)
    : content::NavigationThrottle(navigation_handle),
      brave_vpn_service_(brave_vpn_service) {}

BraveVpnNavigationThrottle::~BraveVpnNavigationThrottle() = default;

content::NavigationThrottle::ThrottleCheckResult
BraveVpnNavigationThrottle::WillStartRequest() {
  GURL url = navigation_handle()->GetURL();

  LOG(ERROR) << "BSC]] url=" << url;

  // TODO(bsclifton): look for a pattern like:
  // https://account.brave.com/?intent=connect-receipt
  //   &product=vpn&order_id=abcdefghijklmnopqrstuvwxyz

  // TODO(bsclifton): detect page; if found, parse out the
  // Order ID number and call submit_receipt (using code in
  // https://github.com/brave/brave-core/pull/13820)
  if (brave_vpn_service_) {
    // TODO: read receipt here
    // ..

    // something like this:
    // maybe should be sku service?
    // brave_vpn_service_->SubmitReceipt(order_id, receipt);
  }

  // return content::NavigationThrottle::DEFER;
  return content::NavigationThrottle::PROCEED;
}

const char* BraveVpnNavigationThrottle::GetNameForLogging() {
  return "BraveVpnNavigationThrottle";
}

}  // namespace brave_vpn
