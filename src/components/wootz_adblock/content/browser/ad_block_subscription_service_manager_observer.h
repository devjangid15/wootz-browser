// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_

#include "base/observer_list_types.h"

namespace wootz_adblock {

class AdBlockSubscriptionServiceManagerObserver : public base::CheckedObserver {
 public:
  ~AdBlockSubscriptionServiceManagerObserver() override = default;
  virtual void OnServiceUpdateEvent() {}
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_
