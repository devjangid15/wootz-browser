// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_WOOTZ_FARBLING_SERVICE_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_WOOTZ_FARBLING_SERVICE_H_

#include "base/memory/raw_ptr.h"
#include "components/keyed_service/core/keyed_service.h"
#include "third_party/abseil-cpp/absl/random/random.h"

class GURL;

class HostContentSettingsMap;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace wootz {

typedef absl::randen_engine<uint64_t> FarblingPRNG;

class WootzFarblingService : public KeyedService {
 public:
  explicit WootzFarblingService(
      HostContentSettingsMap* host_content_settings_map);
  ~WootzFarblingService() override;

  bool MakePseudoRandomGeneratorForURL(const GURL& url, FarblingPRNG* prng);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  const raw_ptr<HostContentSettingsMap> host_content_settings_map_;
};

}  // namespace wootz

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_WOOTZ_FARBLING_SERVICE_H_
