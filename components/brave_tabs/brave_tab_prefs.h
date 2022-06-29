/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_TABS_BRAVE_TAB_PREFS_H_
#define BRAVE_COMPONENTS_BRAVE_TABS_BRAVE_TAB_PREFS_H_

class PrefRegistrySimple;
class Profile;

namespace brave_tabs {

enum TooltipMode {
  CLASSIC = 0,
  CARD = 1,
  CARD_WITH_PREVIEW = 2
};

extern const char kTabTooltipMode[];

void RegisterBraveProfilePrefs(PrefRegistrySimple* registry);

bool AreTooltipsEnabled(Profile* profile);
bool AreCardPreviewsEnabled(Profile* profile);
}

#endif  // BRAVE_COMPONENTS_BRAVE_TABS_BRAVE_TAB_PREFS_H_
