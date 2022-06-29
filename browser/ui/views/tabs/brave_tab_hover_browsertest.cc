// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/time/time.h"
#include "brave/components/brave_tabs/brave_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/tabs/tab_renderer_data.h"
#include "chrome/browser/ui/test/test_browser_dialog.h"
#include "chrome/browser/ui/thumbnails/thumbnail_tab_helper.h"
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
#include "chrome/browser/ui/views/tabs/tab_hover_card_bubble_view.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_controller.h"
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
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/test/browser_test.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/models/list_selection_model.h"
#include "ui/events/event.h"
#include "ui/events/event_constants.h"
#include "ui/events/types/event_type.h"
#include "ui/gfx/animation/animation_test_api.h"
#include "ui/gfx/color_palette.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/favicon_size.h"
#include "ui/gfx/geometry/point.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"
#include "ui/views/layout/flex_layout.h"
#include "ui/views/test/widget_test.h"
#include "ui/views/view_class_properties.h"
#include "ui/views/widget/widget.h"

using views::Widget;

class BraveTabHoverTest : public InProcessBrowserTest {
 public:
  BraveTabHoverTest()
      : animation_mode_reset_(gfx::AnimationTestApi::SetRichAnimationRenderMode(
            gfx::Animation::RichAnimationRenderMode::FORCE_DISABLED)) {
    TabHoverCardController::disable_animations_for_testing_ = true;
  }

  BraveTabHoverTest(const BraveTabHoverTest&) = delete;
  BraveTabHoverTest& operator=(const BraveTabHoverTest&) = delete;

  content::WebContents* contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  void HoverMouseOverTabAt(int index) {
    Tab* const tab = active_tab();
    ui::MouseEvent click_event(ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
                               base::TimeTicks(), ui::EF_NONE, 0);
    tab->OnMousePressed(click_event);
  }

  TabHoverCardBubbleView* hover_card() {
    return tabstrip()->hover_card_controller_->hover_card_;
  }

  TabStrip* tabstrip() {
    auto* browser_view = static_cast<BrowserView*>(browser()->window());
    return browser_view->tabstrip();
  }

  Tab* active_tab() { return tabstrip()->tab_at(tabstrip()->GetActiveIndex()); }

 private:
  std::unique_ptr<base::AutoReset<gfx::Animation::RichAnimationRenderMode>>
      animation_mode_reset_;
};

// There should be no tooltip unless the mode is |TOOLTIP|, as otherwise we'll
// get a tooltip AND a card showing up.
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

// The ThumbnailTabHelper needs to be attached in all |TabHoverModes| so that
// we can change between modes safely without restarting.
IN_PROC_BROWSER_TEST_F(BraveTabHoverTest, ThumbnailHelperIsAlwaysAttached) {
  browser()->profile()->GetPrefs()->SetInteger(brave_tabs::kTabHoverMode,
                                               brave_tabs::TabHoverMode::CARD);
  TabRendererData data;
  data.visible_url = GURL("https://card.com");
  data.title = u"Hello World";
  tabstrip()->AddTabAt(0, data);
  EXPECT_EQ(0, tabstrip()->GetActiveIndex());
  EXPECT_EQ(data.visible_url, active_tab()->data().visible_url);
  EXPECT_NE(nullptr,
            content::WebContentsUserData<ThumbnailTabHelper>::FromWebContents(
                contents()));

  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::CARD_WITH_PREVIEW);
  data.visible_url = GURL("https://card-with-preview.com");
  data.title = u"Foo Bar";
  tabstrip()->AddTabAt(0, data);
  EXPECT_EQ(0, tabstrip()->GetActiveIndex());
  EXPECT_EQ(data.visible_url, active_tab()->data().visible_url);
  EXPECT_NE(nullptr,
            content::WebContentsUserData<ThumbnailTabHelper>::FromWebContents(
                contents()));

  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::TOOLTIP);

  data.visible_url = GURL("https://tooltip.com");
  data.title = u"Baf Baz";
  tabstrip()->AddTabAt(0, data);
  EXPECT_EQ(0, tabstrip()->GetActiveIndex());
  EXPECT_EQ(data.visible_url, active_tab()->data().visible_url);
  EXPECT_NE(nullptr,
            content::WebContentsUserData<ThumbnailTabHelper>::FromWebContents(
                contents()));
}

IN_PROC_BROWSER_TEST_F(BraveTabHoverTest,
                       ThumbnailViewIsCreatedInCardPreviewMode) {
  // In Card mode, the widget should become visible but the thumbnail should not
  // be created.
  browser()->profile()->GetPrefs()->SetInteger(brave_tabs::kTabHoverMode,
                                               brave_tabs::TabHoverMode::CARD);
  tabstrip()->GetFocusManager()->SetFocusedView(active_tab());
  Widget* widget = hover_card()->GetWidget();
  ASSERT_NE(nullptr, widget);
  views::test::WidgetVisibleWaiter(widget).Wait();
  EXPECT_FALSE(hover_card()->has_thumbnail_view());

  // Clear focus, to hide the bubble.
  tabstrip()->GetFocusManager()->SetFocusedView(nullptr);
  EXPECT_FALSE(widget->IsVisible());

  // In Preview mode, the widget should become visible and the card should have
  // a thumbnail view.
  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::CARD_WITH_PREVIEW);
  tabstrip()->GetFocusManager()->SetFocusedView(active_tab());
  widget = hover_card()->GetWidget();
  ASSERT_NE(nullptr, widget);
  views::test::WidgetVisibleWaiter(widget).Wait();
  EXPECT_TRUE(hover_card()->has_thumbnail_view());

  // Clear focus, to hide the bubble.
  tabstrip()->GetFocusManager()->SetFocusedView(nullptr);
  EXPECT_FALSE(widget->IsVisible());

  // In Tooltip mode, the widget should not be made visible.
  browser()->profile()->GetPrefs()->SetInteger(
      brave_tabs::kTabHoverMode, brave_tabs::TabHoverMode::TOOLTIP);
  tabstrip()->GetFocusManager()->SetFocusedView(active_tab());
  widget = hover_card()->GetWidget();
  ASSERT_NE(nullptr, widget);
  EXPECT_FALSE(widget->IsVisible());
}
