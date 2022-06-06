/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_rewards/rewards_panel_coordinator.h"

#include <memory>
#include <string>
#include <utility>

#include "base/feature_list.h"
#include "base/memory/weak_ptr.h"
#include "brave/components/brave_rewards/common/features.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/test/base/browser_with_test_window_test.h"

namespace brave_rewards {

namespace {

template <typename F>
class PanelDelegate : public RewardsPanelCoordinator::Delegate {
 public:
  explicit PanelDelegate(F f) : f_(std::move(f)) {}
  ~PanelDelegate() override {}

  bool OpenRewardsPanel(const mojom::RewardsPanelArgs& args) override {
    return f_(args);
  }

  base::WeakPtr<PanelDelegate> GetWeakPtr() {
    return weak_factory_.GetWeakPtr();
  }

 private:
  F f_;
  base::WeakPtrFactory<PanelDelegate> weak_factory_{this};
};

template <typename F>
auto MakePanelDelegate(F&& f) {
  return std::make_unique<PanelDelegate<F>>(std::forward<F>(f));
}

}  // namespace

class RewardsPanelCoordinatorTest : public BrowserWithTestWindowTest {
 public:
  void SetUp() override {
    base::test::ScopedFeatureList features;
    features.InitWithFeatures({features::kWebUIPanelFeature}, {});
    BrowserWithTestWindowTest::SetUp();

    auto delegate =
        MakePanelDelegate([this](const mojom::RewardsPanelArgs& args) {
          last_args_ = args.Clone();
          return true;
        });

    coordinator_ = RewardsPanelCoordinator::RegisterForBrowser(browser());
    DCHECK(coordinator_);
    coordinator_->SetDelegate(delegate->GetWeakPtr());
    delegate_ = std::move(delegate);
  }

 protected:
  const mojom::RewardsPanelArgs& last_args() const {
    CHECK(last_args_);
    return *last_args_;
  }

  RewardsPanelCoordinator& coordinator() { return *coordinator_; }

 private:
  mojom::RewardsPanelArgsPtr last_args_;
  std::unique_ptr<RewardsPanelCoordinator::Delegate> delegate_;
  raw_ptr<RewardsPanelCoordinator> coordinator_ = nullptr;
};

TEST_F(RewardsPanelCoordinatorTest, OpenRewardsPanel) {
  EXPECT_TRUE(coordinator().OpenRewardsPanel());
  EXPECT_EQ(last_args().view, mojom::RewardsPanelView::kDefault);
  EXPECT_EQ(last_args().data, "");
}

TEST_F(RewardsPanelCoordinatorTest, ShowRewardsTour) {
  EXPECT_TRUE(coordinator().ShowRewardsTour());
  EXPECT_EQ(last_args().view, mojom::RewardsPanelView::kRewardsTour);
  EXPECT_EQ(last_args().data, "");
}

TEST_F(RewardsPanelCoordinatorTest, ShowGrantCaptcha) {
  EXPECT_TRUE(coordinator().ShowGrantCaptcha("abc123"));
  EXPECT_EQ(last_args().view, mojom::RewardsPanelView::kGrantCaptcha);
  EXPECT_EQ(last_args().data, "abc123");
}

TEST_F(RewardsPanelCoordinatorTest, ShowAdaptiveCaptcha) {
  EXPECT_TRUE(coordinator().ShowAdaptiveCaptcha());
  EXPECT_EQ(last_args().view, mojom::RewardsPanelView::kAdaptiveCaptcha);
  EXPECT_EQ(last_args().data, "");
}

TEST_F(RewardsPanelCoordinatorTest, ShowBraveTalkOptIn) {
  EXPECT_TRUE(coordinator().ShowBraveTalkOptIn());
  EXPECT_EQ(last_args().view, mojom::RewardsPanelView::kBraveTalkOptIn);
  EXPECT_EQ(last_args().data, "");
}

}  // namespace brave_rewards
