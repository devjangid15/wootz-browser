// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_COMPONENT_FILTERS_PROVIDER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_COMPONENT_FILTERS_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/core/browser/ad_block_filters_provider.h"

using wootz_component_updater::DATFileDataBuffer;

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace base {
class FilePath;
}

class AdBlockServiceTest;
class DebounceBrowserTest;

namespace wootz_adblock {

class FilterListCatalogEntry;

class AdBlockComponentFiltersProvider : public AdBlockFiltersProvider {
 public:
  // Build an adblock filters component with given component info
  AdBlockComponentFiltersProvider(
      component_updater::ComponentUpdateService* cus,
      std::string component_id,
      std::string base64_public_key,
      std::string title,
      uint8_t permission_mask,
      bool is_default_engine = true);
  // Helper to build a particular adblock component from a catalog entry
  AdBlockComponentFiltersProvider(
      component_updater::ComponentUpdateService* cus,
      const FilterListCatalogEntry& catalog_entry,
      bool is_default_engine = true);
  ~AdBlockComponentFiltersProvider() override;
  AdBlockComponentFiltersProvider(const AdBlockComponentFiltersProvider&) =
      delete;
  AdBlockComponentFiltersProvider& operator=(
      const AdBlockComponentFiltersProvider&) = delete;

  const std::string& component_id() const { return component_id_; }

  // Return a file path to the filter set if its available
  // Otherwise an empty path is returned
  base::FilePath GetFilterSetPath();

  void LoadFilterSet(
      base::OnceCallback<void(
          base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)>) override;

  // Remove the component. This will force it to be redownloaded next time it
  // is registered.
  void UnregisterComponent();

  std::string GetNameForDebugging() override;

  bool IsInitialized() const override;

 private:
  friend class ::AdBlockServiceTest;
  friend class ::DebounceBrowserTest;

  void OnComponentReady(const base::FilePath&);

  base::FilePath component_path_;
  std::string component_id_;
  uint8_t permission_mask_;
  const raw_ptr<component_updater::ComponentUpdateService>
      component_updater_service_;

  base::WeakPtrFactory<AdBlockComponentFiltersProvider> weak_factory_{this};
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_COMPONENT_FILTERS_PROVIDER_H_
