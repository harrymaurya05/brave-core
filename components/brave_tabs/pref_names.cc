/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_tabs/pref_names.h"
#include <cstddef>

#include "base/feature_list.h"
#include "components/prefs/pref_registry_simple.h"

namespace brave_tabs {
const char kTabHoverCardTooltipsEnabled[] =
    "brave.tabs.hover_card_tooltips_enabled";
const char kTabHoverCardPreviewEnabled[] =
    "brave.tabs.hover_card_preview_enabled";

void RegisterBraveProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kTabHoverCardTooltipsEnabled, true);
  registry->RegisterBooleanPref(kTabHoverCardPreviewEnabled, false);
}
}  // namespace brave_tabs
