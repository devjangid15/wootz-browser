// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_TEST_TEST_FILTERS_PROVIDER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_TEST_TEST_FILTERS_PROVIDER_H_

#include <string>
#include <vector>

#include "base/functional/callback.h"
#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/core/browser/ad_block_filters_provider.h"

namespace wootz_adblock {

class TestFiltersProvider : public AdBlockFiltersProvider {
 public:
  explicit TestFiltersProvider(const std::string& rules);
  TestFiltersProvider(const std::string& rules,
                      bool engine_is_default,
                      uint8_t permission_mask = 0,
                      bool is_initialized = true);
  ~TestFiltersProvider() override;

  void LoadFilterSet(
      base::OnceCallback<void(
          base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)>) override;

  void Initialize();
  bool IsInitialized() const override;

  std::string GetNameForDebugging() override;

 private:
  std::string rules_;
  uint8_t permission_mask_;
  bool is_initialized_;
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_TEST_TEST_FILTERS_PROVIDER_H_
