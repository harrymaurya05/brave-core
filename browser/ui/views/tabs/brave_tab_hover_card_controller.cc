#include "brave/browser/ui/views/tabs/brave_tab_hover_card_controller.h"
#include <memory>

#include "base/bind.h"
#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_controller.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_thumbnail_observer.h"

BraveTabHoverCardController::~BraveTabHoverCardController() = default;

void BraveTabHoverCardController::CreateHoverCard(Tab* tab) {
  TabHoverCardController::CreateHoverCard(tab);

  if (!thumbnail_observer_ &&
      brave_tabs::AreCardPreviewsEnabled(
          tab->controller()->GetBrowser()->profile()->GetPrefs())) {
    thumbnail_observer_ = std::make_unique<TabHoverCardThumbnailObserver>();
    thumbnail_subscription_ = thumbnail_observer_->AddCallback(
        base::BindRepeating(&TabHoverCardController::OnPreviewImageAvaialble,
                            weak_ptr_factory_.GetWeakPtr()));
  }
}
