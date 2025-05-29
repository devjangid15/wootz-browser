// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_FILTERS_PROVIDER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_FILTERS_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "components/wootz_component_updater/browser/dat_file_util.h"
#include "components/wootz_adblock/adblock/rs/src/lib.rs.h"

using wootz_component_updater::DATFileDataBuffer;

namespace wootz_adblock {

// Interface for any source that can load filters or serialized filter data
// into an adblock engine.
class AdBlockFiltersProvider {
 public:
  class Observer : public base::CheckedObserver {
   public:
    virtual void OnChanged(bool is_for_default_engine) = 0;
  };

  explicit AdBlockFiltersProvider(bool engine_is_default);
  // Used by AdblockFiltersProviderManager
  AdBlockFiltersProvider();
  AdBlockFiltersProvider(const AdBlockFiltersProvider&) = delete;
  AdBlockFiltersProvider& operator=(const AdBlockFiltersProvider&) = delete;
  virtual ~AdBlockFiltersProvider();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  virtual void LoadFilterSet(
      base::OnceCallback<
          void(base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)>) = 0;

  base::WeakPtr<AdBlockFiltersProvider> AsWeakPtr();

  virtual std::string GetNameForDebugging() = 0;

  // Intended to be overridden if the provider implementation is not immediately
  // ready at creation time.
  virtual bool IsInitialized() const;

 protected:
  bool engine_is_default_;

  void NotifyObservers(bool is_for_default_engine);

 private:
  base::ObserverList<Observer> observers_;
  base::WeakPtrFactory<AdBlockFiltersProvider> weak_factory_{this};
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_FILTERS_PROVIDER_H_
