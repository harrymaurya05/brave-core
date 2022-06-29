/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_

#define PreventImmediateReshow     \
  PreventImmediateReshow_Unused(); \
  friend class BraveTabHoverTest;  \
  void PreventImmediateReshow

#include "src/chrome/browser/ui/views/tabs/tab_hover_card_controller.h"

#undef PreventImmediateReshow

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
