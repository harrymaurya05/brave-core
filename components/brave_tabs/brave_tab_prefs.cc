/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_tabs/brave_tab_prefs.h"

#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace brave_tabs {

const char kTabTooltipMode[] = "brave.tabs.tooltip_mode";

void RegisterBraveProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterIntegerPref(kTabTooltipMode, TooltipMode::CARD);
}

bool AreTooltipsEnabled(Profile* profile) {
  return profile->GetPrefs()->GetInteger(kTabTooltipMode) == TooltipMode::CLASSIC;
}

bool AreCardPreviewsEnabled(Profile* profile) {
  return profile->GetPrefs()->GetInteger(kTabTooltipMode) ==
         TooltipMode::CARD_WITH_PREVIEW;
}

}  // namespace brave_tabs
