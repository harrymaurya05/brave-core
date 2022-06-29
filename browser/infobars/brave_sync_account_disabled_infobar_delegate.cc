/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/infobars/brave_sync_account_disabled_infobar_delegate.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
#include "brave/browser/ui/brave_pages.h"
#include "brave/components/brave_sync/brave_sync_prefs.h"
#include "brave/components/constants/pref_names.h"
#include "brave/components/l10n/common/locale_util.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/grit/chromium_strings.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "ui/views/vector_icons.h"

// static
void BraveSyncAccountDisabledInfoBarDelegate::Create(
    infobars::ContentInfoBarManager* infobar_manager,
    Profile* profile,
    Browser* browser) {
  brave_sync::Prefs brave_sync_prefs(profile->GetPrefs());
  const bool has_dismissed = brave_sync_prefs.IsSyncMigrateNoticeDismissed();
  if (has_dismissed) {
    return;
  }

  // Show infobar
  infobar_manager->AddInfoBar(
      CreateConfirmInfoBar(std::unique_ptr<ConfirmInfoBarDelegate>(
          new BraveSyncAccountDisabledInfoBarDelegate(browser, profile))));
}

// Start class impl
BraveSyncAccountDisabledInfoBarDelegate::
    BraveSyncAccountDisabledInfoBarDelegate(Browser* browser, Profile* profile)
    : ConfirmInfoBarDelegate(), profile_(profile), browser_(browser) {}

BraveSyncAccountDisabledInfoBarDelegate::
    ~BraveSyncAccountDisabledInfoBarDelegate() {}

infobars::InfoBarDelegate::InfoBarIdentifier
BraveSyncAccountDisabledInfoBarDelegate::GetIdentifier() const {
  return BRAVE_SYNC_ACCOUNT_DISABLED_INFOBAR;
}

const gfx::VectorIcon& BraveSyncAccountDisabledInfoBarDelegate::GetVectorIcon()
    const {
  return views::kInfoIcon;
}

bool BraveSyncAccountDisabledInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void BraveSyncAccountDisabledInfoBarDelegate::InfoBarDismissed() {
  brave_sync::Prefs brave_sync_prefs(profile_->GetPrefs());
  brave_sync_prefs.SetDismissSyncMigrateNotice(true);
}

std::u16string BraveSyncAccountDisabledInfoBarDelegate::GetMessageText() const {
  return brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SYNC_ACCOUNT_DELETED_INFOBAR_MESSAGE);
}

int BraveSyncAccountDisabledInfoBarDelegate::GetButtons() const {
  return BUTTON_OK;
}

std::u16string BraveSyncAccountDisabledInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SYNC_ACCOUNT_DELETED_INFOBAR_COMMAND);
}

bool BraveSyncAccountDisabledInfoBarDelegate::Accept() {
  brave::ShowSync(browser_);
  return true;
}
