// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/content/browser/wootz_farbling_service.h"

#include "components/wootz_adblock/content/browser/wootz_adblock_util.h"
#include "components/wootz_adblock/core/common/pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "url/gurl.h"

namespace wootz {

WootzFarblingService::WootzFarblingService(
    HostContentSettingsMap* host_content_settings_map)
    : host_content_settings_map_(host_content_settings_map) {
  DCHECK(host_content_settings_map_);
}

WootzFarblingService::~WootzFarblingService() = default;

bool WootzFarblingService::MakePseudoRandomGeneratorForURL(const GURL& url,
                                                           FarblingPRNG* prng) {
  if (wootz_adblock::GetFarblingLevel(host_content_settings_map_, url) ==
      wootz_adblock::mojom::FarblingLevel::OFF) {
    return false;
  }
  const base::Token farbling_token =
      wootz_adblock::GetFarblingToken(host_content_settings_map_, url);
  if (farbling_token.is_zero()) {
    return false;
  }
  *prng = FarblingPRNG(farbling_token.high() ^ farbling_token.low());
  return true;
}

// static
void WootzFarblingService::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(wootz_adblock::prefs::kReduceLanguageEnabled,
                                true);
}

}  // namespace wootz
