#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_

#define PreventImmediateReshow     \
  PreventImmediateReshow_Unused(); \
  friend class BraveTabHoverTest;  \
  void PreventImmediateReshow

#include "src/chrome/browser/ui/views/tabs/tab_hover_card_controller.h"

#undef PreventImmediateReshow

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
