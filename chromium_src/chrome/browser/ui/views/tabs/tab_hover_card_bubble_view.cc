/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_bubble_view.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"
#include "content/public/common/url_constants.h"
#include "ui/views/controls/label.h"

#define BRAVE_TAB_HOVER_CARD_PREVIEWS    \
  || brave_tabs::AreCardPreviewsEnabled( \
         tab->controller()->GetBrowser()->profile())
#define TabHoverCardBubbleView TabHoverCardBubbleView_ChromiumImpl
#include "src/chrome/browser/ui/views/tabs/tab_hover_card_bubble_view.cc"
#undef TabHoverCardBubbleView
#undef BRAVE_TAB_HOVER_CARD_PREVIEWS

void TabHoverCardBubbleView_ChromiumImpl::BraveUpdateCardContent(
    const Tab* tab) {
  TabHoverCardBubbleView_ChromiumImpl::UpdateCardContent(tab);
  const std::u16string& domain = domain_label_->GetText();
  const std::u16string kChromeUISchemeU16 =
      base::ASCIIToUTF16(base::StrCat({content::kChromeUIScheme, "://"}));
  // Replace chrome:// with brave://. Since this is purely in the UI we can
  // just do a sub-string replacement instead of parsing into GURL.
  if (base::StartsWith(domain, kChromeUISchemeU16,
                       base::CompareCase::INSENSITIVE_ASCII)) {
    std::u16string new_domain = domain;
    base::ReplaceFirstSubstringAfterOffset(
        &new_domain, 0ul, kChromeUISchemeU16,
        base::ASCIIToUTF16(base::StrCat({content::kBraveUIScheme, "://"})));
    domain_label_->SetText(new_domain, /*is_filename*/ false);
  }
}

void TabHoverCardBubbleView::UpdateCardContent(const Tab* tab) {
  BraveUpdateCardContent(tab);
}

void TabHoverCardBubbleView::SetTargetTabImage(gfx::ImageSkia preview_image) {
  if (!has_thumbnail_view())
    return;
  TabHoverCardBubbleView_ChromiumImpl::SetTargetTabImage(preview_image);
}
