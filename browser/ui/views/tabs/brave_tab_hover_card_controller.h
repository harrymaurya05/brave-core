#ifndef BRAVE_BROWSER_UI_VIEWS_TABS_BRAVE_TAB_HOVER_CARD_CONTROLLER_H_
#define BRAVE_BROWSER_UI_VIEWS_TABS_BRAVE_TAB_HOVER_CARD_CONTROLLER_H_

#include "chrome/browser/ui/views/tabs/tab_hover_card_controller.h"

class Tab;

class BraveTabHoverCardController : public TabHoverCardController {
 public:
  using TabHoverCardController::TabHoverCardController;
  ~BraveTabHoverCardController() override;

 protected:
  void CreateHoverCard(Tab* tab) override;
};

#endif  // BRAVE_BROWSER_UI_VIEWS_TABS_BRAVE_TAB_HOVER_CARD_CONTROLLER_H_
