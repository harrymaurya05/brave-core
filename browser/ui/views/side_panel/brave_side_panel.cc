// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/views/side_panel/brave_side_panel.h"

#include <memory>

#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/border.h"
#include "ui/views/layout/fill_layout.h"

BraveSidePanel::BraveSidePanel(BrowserView* browser_view) {
  SetVisible(false);
  SetLayoutManager(std::make_unique<views::FillLayout>());

  // TODO(pbos): Reconsider if SetPanelWidth() should add borders, if so move
  // accounting for the border into SetPanelWidth(), otherwise remove this TODO.
  constexpr int kDefaultWidth = 320;
  SetPreferredSize(gfx::Size(kDefaultWidth, 1));
  SetBorder(views::CreateEmptyBorder(gfx::Insets::TLBR(-1, 0, 0, 0)));

  AddObserver(this);
}

BraveSidePanel::~BraveSidePanel() {
  RemoveObserver(this);
}

void BraveSidePanel::ChildVisibilityChanged(View* child) {
  UpdateVisibility();
}

void BraveSidePanel::OnChildViewAdded(View* observed_view, View* child) {
  UpdateVisibility();
}

void BraveSidePanel::OnChildViewRemoved(View* observed_view, View* child) {
  UpdateVisibility();
}

void BraveSidePanel::UpdateVisibility() {
  bool any_child_visible = false;
  for (const auto* view : children()) {
    if (view->GetVisible()) {
      any_child_visible = true;
      break;
    }
  }
  SetVisible(any_child_visible);
}

BEGIN_METADATA(BraveSidePanel, views::View)
END_METADATA
