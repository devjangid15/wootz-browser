// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/core/browser/ad_block_resource_provider.h"

#include <string>

namespace wootz_adblock {

AdBlockResourceProvider::AdBlockResourceProvider() = default;

AdBlockResourceProvider::~AdBlockResourceProvider() = default;

void AdBlockResourceProvider::AddObserver(
    AdBlockResourceProvider::Observer* observer) {
  if (!observers_.HasObserver(observer)) {
    observers_.AddObserver(observer);
  }
}

void AdBlockResourceProvider::RemoveObserver(
    AdBlockResourceProvider::Observer* observer) {
  if (observers_.HasObserver(observer)) {
    observers_.RemoveObserver(observer);
  }
}

void AdBlockResourceProvider::NotifyResourcesLoaded(
    const std::string& resources_json) {
  for (auto& observer : observers_) {
    observer.OnResourcesLoaded(resources_json);
  }
}

}  // namespace wootz_adblock
