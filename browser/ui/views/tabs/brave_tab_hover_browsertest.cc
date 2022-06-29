// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/tabs/tab_renderer_data.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/tabs/tab.h"

#include <stddef.h>

#include <memory>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/simple_test_tick_clock.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/tabs/tab_enums.h"
#include "chrome/browser/ui/tabs/tab_types.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "chrome/browser/ui/views/tabs/alert_indicator_button.h"
#include "chrome/browser/ui/views/tabs/fake_base_tab_strip_controller.h"
#include "chrome/browser/ui/views/tabs/fake_tab_slot_controller.h"
#include "chrome/browser/ui/views/tabs/tab_close_button.h"
#include "chrome/browser/ui/views/tabs/tab_icon.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"
#include "chrome/browser/ui/views/tabs/tab_slot_view.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"
#include "chrome/browser/ui/views/tabs/tab_style_views.h"
#include "chrome/common/chrome_features.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/views/chrome_views_test_base.h"
#include "components/prefs/pref_service.h"
#include "components/tab_groups/tab_group_id.h"
#include "components/tab_groups/tab_group_visual_data.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/models/list_selection_model.h"
#include "ui/gfx/color_palette.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/favicon_size.h"
#include "ui/gfx/geometry/point.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"
#include "ui/views/layout/flex_layout.h"
#include "ui/views/view_class_properties.h"
#include "ui/views/widget/widget.h"

using views::Widget;

class BraveTabHoverTest : public InProcessBrowserTest {
 public:
  BraveTabHoverTest() {}
  BraveTabHoverTest(const BraveTabHoverTest&) = delete;
  BraveTabHoverTest& operator=(const BraveTabHoverTest&) = delete;

  void SetUpOnMainThread() override {}

  void SetUp() override { InProcessBrowserTest::SetUp(); }

  content::WebContents* contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  TabStrip* tabstrip() {
    auto* browser_view = static_cast<BrowserView*>(browser()->window());
    return browser_view->tabstrip();
  }

  Tab* active_tab() { return tabstrip()->tab_at(tabstrip()->GetActiveIndex()); }
};

IN_PROC_BROWSER_TEST_F(BraveTabHoverTest,
                       GetTooltipOnlyHasTextWhenHoverModeIsTooltip) {
  TabRendererData data;
  data.visible_url = GURL("https://example.com");
  data.title = u"Hello World";
  tabstrip()->SetTabData(tabstrip()->GetActiveIndex(), data);
  EXPECT_EQ(u"Hello World", active_tab()->data().title);

  browser()->profile()->GetPrefs()->SetInteger(brave_tabs::kTabHoverMode,
                                               brave_tabs::TabHoverMode::CARD);
  EXPECT_EQ(u"", active_tab()->GetTooltipText(gfx::Point()));

  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::CARD_WITH_PREVIEW);
  EXPECT_EQ(u"", active_tab()->GetTooltipText(gfx::Point()));

  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::TOOLTIP);
  EXPECT_EQ(u"Hello World", active_tab()->GetTooltipText(gfx::Point()));
}
