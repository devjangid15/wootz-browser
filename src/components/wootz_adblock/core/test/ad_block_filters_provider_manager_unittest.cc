// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/core/browser/ad_block_filters_provider_manager.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "components/wootz_adblock/content/test/test_filters_provider.h"

class FiltersProviderManagerTestObserver
    : public wootz_adblock::AdBlockFiltersProvider::Observer {
 public:
  FiltersProviderManagerTestObserver() = default;

  // AdBlockFiltersProvider::Observer
  void OnChanged(bool is_default_engine) override { changed_count += 1; }

  int changed_count = 0;
};

TEST(AdBlockFiltersProviderManagerTest, WaitUntilInitialized) {
  FiltersProviderManagerTestObserver test_observer;
  wootz_adblock::AdBlockFiltersProviderManager::GetInstance()->AddObserver(
      &test_observer);

  wootz_adblock::TestFiltersProvider provider1("", true, 0, false);
  wootz_adblock::TestFiltersProvider provider2("", true, 0, false);
  wootz_adblock::TestFiltersProvider provider3("", true, 0, false);

  provider1.Initialize();
  EXPECT_EQ(test_observer.changed_count, 0);
  provider2.Initialize();
  EXPECT_EQ(test_observer.changed_count, 0);
  provider3.Initialize();
  EXPECT_EQ(test_observer.changed_count, 1);
}
