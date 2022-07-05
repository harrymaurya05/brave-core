/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_NAVIGATION_THROTTLE_H_
#define BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_NAVIGATION_THROTTLE_H_

#include <memory>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/navigation_throttle.h"

namespace content {
class NavigationHandle;
}  // namespace content

namespace brave_vpn {

class BraveVpnService;

class BraveVpnNavigationThrottle : public content::NavigationThrottle {
 public:
  explicit BraveVpnNavigationThrottle(
      content::NavigationHandle* navigation_handle,
      BraveVpnService* brave_vpn_service);
  ~BraveVpnNavigationThrottle() override;

  BraveVpnNavigationThrottle(const BraveVpnNavigationThrottle&) = delete;
  BraveVpnNavigationThrottle& operator=(const BraveVpnNavigationThrottle&) =
      delete;

  static std::unique_ptr<BraveVpnNavigationThrottle> MaybeCreateThrottleFor(
      content::NavigationHandle* navigation_handle,
      BraveVpnService* brave_vpn_service);

  // content::NavigationThrottle implementation:
  ThrottleCheckResult WillStartRequest() override;
  const char* GetNameForLogging() override;

 private:
  // TODO: friend for unit tests
  // ...

  BraveVpnService* brave_vpn_service_ = nullptr;
  base::WeakPtrFactory<BraveVpnNavigationThrottle> weak_ptr_factory_{this};
};

}  // namespace brave_vpn

#endif  // BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_NAVIGATION_THROTTLE_H_
