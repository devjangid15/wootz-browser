// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/core/browser/ad_block_filters_provider.h"

#include <utility>

#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/core/browser/ad_block_filters_provider_manager.h"

namespace wootz_adblock {

AdBlockFiltersProvider::AdBlockFiltersProvider(bool engine_is_default)
    : engine_is_default_(engine_is_default) {
  AdBlockFiltersProviderManager::GetInstance()->AddProvider(this,
                                                            engine_is_default_);
}

AdBlockFiltersProvider::AdBlockFiltersProvider() = default;

AdBlockFiltersProvider::~AdBlockFiltersProvider() {
  AdBlockFiltersProviderManager::GetInstance()->RemoveProvider(
      this, engine_is_default_);
}

void AdBlockFiltersProvider::AddObserver(
    AdBlockFiltersProvider::Observer* observer) {
  if (!observers_.HasObserver(observer)) {
    observers_.AddObserver(observer);
  }
}

void AdBlockFiltersProvider::RemoveObserver(
    AdBlockFiltersProvider::Observer* observer) {
  if (observers_.HasObserver(observer)) {
    observers_.RemoveObserver(observer);
  }
}

void AdBlockFiltersProvider::NotifyObservers(bool is_for_default_engine) {
  for (auto& observer : observers_) {
    observer.OnChanged(is_for_default_engine);
  }
}

bool AdBlockFiltersProvider::IsInitialized() const {
  return true;
}

base::WeakPtr<AdBlockFiltersProvider> AdBlockFiltersProvider::AsWeakPtr() {
  return weak_factory_.GetWeakPtr();
}

}  // namespace wootz_adblock
