// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_FILTERS_PROVIDER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_FILTERS_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/core/browser/ad_block_filters_provider.h"

using wootz_component_updater::DATFileDataBuffer;

class PrefService;

namespace adblock {
struct FilterListMetadata;
}  // namespace adblock

namespace wootz_adblock {

class AdBlockSubscriptionFiltersProvider : public AdBlockFiltersProvider {
 public:
  AdBlockSubscriptionFiltersProvider(
      PrefService* local_state,
      base::FilePath list_file,
      base::RepeatingCallback<void(const adblock::FilterListMetadata&)>
          on_metadata_retrieved);
  AdBlockSubscriptionFiltersProvider(
      const AdBlockSubscriptionFiltersProvider&) = delete;
  AdBlockSubscriptionFiltersProvider& operator=(
      const AdBlockSubscriptionFiltersProvider&) = delete;
  ~AdBlockSubscriptionFiltersProvider() override;

  void LoadFilterSet(
      base::OnceCallback<void(
          base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)>) override;

  void OnListAvailable();

 private:
  void OnDATFileDataReady(
      base::OnceCallback<
          void(base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)> cb,
      const DATFileDataBuffer& dat_buf);

  std::string GetNameForDebugging() override;

 private:
  base::FilePath list_file_;

  base::RepeatingCallback<void(const adblock::FilterListMetadata&)>
      on_metadata_retrieved_;

  base::WeakPtrFactory<AdBlockSubscriptionFiltersProvider> weak_factory_{this};
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_FILTERS_PROVIDER_H_
