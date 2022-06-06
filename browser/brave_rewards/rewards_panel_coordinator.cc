/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_rewards/rewards_panel_coordinator.h"

#include <string>
#include <utility>

#include "base/feature_list.h"
#include "brave/browser/brave_rewards/rewards_panel_extension_handler.h"
#include "brave/browser/profiles/profile_util.h"
#include "brave/components/brave_rewards/common/features.h"
#include "brave/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window.h"

namespace brave_rewards {

RewardsPanelCoordinator::Delegate::~Delegate() = default;

RewardsPanelCoordinator::RewardsPanelCoordinator(Browser* browser)
    : BrowserUserData<RewardsPanelCoordinator>(*browser) {
  // If we are using the Rewards extension to display the Rewards panel, then
  // create an instance of `RewardsPanelExtensionHandler` to load the extension
  // and dispatch panel requests to the extension.
  if (!base::FeatureList::IsEnabled(features::kWebUIPanelFeature)) {
    auto handler =
        std::make_unique<RewardsPanelExtensionHandler>(&GetBrowser());
    SetDelegate(handler->GetWeakPtr());
    extension_handler_ = std::move(handler);
  }
}

RewardsPanelCoordinator::~RewardsPanelCoordinator() = default;

bool RewardsPanelCoordinator::IsRewardsPanelURLForTesting(const GURL& url) {
  if (base::FeatureList::IsEnabled(features::kWebUIPanelFeature)) {
    return url.host() == kBraveRewardsPanelHost;
  } else {
    return RewardsPanelExtensionHandler::IsRewardsExtensionPanelURL(url);
  }
}

RewardsPanelCoordinator* RewardsPanelCoordinator::RegisterForBrowser(
    Browser* browser) {
  DCHECK(browser);
  if (brave::IsRegularProfile(browser->profile())) {
    return GetOrCreateForBrowser(browser);
  }
  return nullptr;
}

bool RewardsPanelCoordinator::OpenRewardsPanel() {
  return OpenWithArgs(
      mojom::RewardsPanelArgs(mojom::RewardsPanelView::kDefault, ""));
}

bool RewardsPanelCoordinator::ShowRewardsTour() {
  return OpenWithArgs(
      mojom::RewardsPanelArgs(mojom::RewardsPanelView::kRewardsTour, ""));
}

bool RewardsPanelCoordinator::ShowGrantCaptcha(const std::string& grant_id) {
  return OpenWithArgs(mojom::RewardsPanelArgs(
      mojom::RewardsPanelView::kGrantCaptcha, grant_id));
}

bool RewardsPanelCoordinator::ShowAdaptiveCaptcha() {
  return OpenWithArgs(
      mojom::RewardsPanelArgs(mojom::RewardsPanelView::kAdaptiveCaptcha, ""));
}

bool RewardsPanelCoordinator::ShowBraveTalkOptIn() {
  return OpenWithArgs(
      mojom::RewardsPanelArgs(mojom::RewardsPanelView::kBraveTalkOptIn, ""));
}

bool RewardsPanelCoordinator::OpenWithArgs(mojom::RewardsPanelArgs&& args) {
  if (GetBrowser().window()->IsMinimized()) {
    GetBrowser().window()->Restore();
  }
  panel_args_ = std::move(args);
  return delegate_ ? delegate_->OpenRewardsPanel(panel_args_) : false;
}

BROWSER_USER_DATA_KEY_IMPL(RewardsPanelCoordinator);

}  // namespace brave_rewards
