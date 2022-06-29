/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/views/tabs/tab_strip.h"
#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

#define BRAVE_UPDATE_HOVER_CARD_                               \
  if (!brave_tabs::UseCardTooltips(controller_->GetProfile())) \
    return;

#include "src/chrome/browser/ui/views/tabs/tab_strip.cc"

#undef UpdateHoverCard
