// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/sequence_checker.h"
#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/core/browser/ad_block_filters_provider.h"

using wootz_component_updater::DATFileDataBuffer;

namespace wootz_adblock {

class AdBlockLocalhostFiltersProvider : public AdBlockFiltersProvider {
 public:
  AdBlockLocalhostFiltersProvider();
  ~AdBlockLocalhostFiltersProvider() override;
  AdBlockLocalhostFiltersProvider(const AdBlockLocalhostFiltersProvider&) =
      delete;
  AdBlockLocalhostFiltersProvider& operator=(
      const AdBlockLocalhostFiltersProvider&) = delete;

  std::string GetLocalhostFilters();
  bool UpdateLocalhostFilters(const std::string& localhost_filters);

  // AdBlockFiltersProvider
  void LoadFilterSet(
      base::OnceCallback<void(
          base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)>) override;

  std::string GetNameForDebugging() override;

 private:
  SEQUENCE_CHECKER(sequence_checker_);
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_
