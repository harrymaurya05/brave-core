#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_

#define FrameColorsChanged        \
  FrameColorsChanged_Unused();    \
  friend class BraveTabHoverTest; \
  void FrameColorsChanged

#include "src/chrome/browser/ui/views/tabs/tab_strip.h"

#undef FrameColorsChanged

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
