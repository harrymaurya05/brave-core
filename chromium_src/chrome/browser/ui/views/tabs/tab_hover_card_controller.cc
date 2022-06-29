/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"

#define BRAVE_TAB_HOVER_CARD_PREVIEWS                             \
  || (!thumbnail_observer_ && brave_tabs::AreCardPreviewsEnabled( \
                                  tab->controller()->GetBrowser()->profile()))

#include "src/chrome/browser/ui/views/tabs/tab_hover_card_controller.cc"

#undef BRAVE_TAB_HOVER_CARD_PREVIEWS
