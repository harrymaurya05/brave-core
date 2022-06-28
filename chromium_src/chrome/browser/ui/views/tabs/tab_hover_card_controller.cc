#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"

#define BRAVE_TAB_HOVER_CARD_PREVIEWS                         \
  || (!thumbnail_observer_ && brave_tabs::ArePreviewsEnabled( \
                                  tab->controller()->GetBrowser()->profile()))

#include "src/chrome/browser/ui/views/tabs/tab_hover_card_controller.cc"

#undef BRAVE_TAB_HOVER_CARD_PREVIEWS
