// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_UTILS_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_UTILS_H_

#include <map>

#include "components/content_settings/core/common/content_settings.h"

class GURL;

namespace wootz_adblock {

struct ShieldsSettingCounts {
  int allow;
  int standard;
  int aggressive;
};

ContentSetting GetWootzFPContentSettingFromRules(
    const ContentSettingsForOneType& fp_rules,
    const GURL& primary_url);

ContentSetting GetWootzWebcompatContentSettingFromRules(
    const std::map<ContentSettingsType, ContentSettingsForOneType>&
        webcompat_rules,
    const GURL& primary_url,
    const ContentSettingsType content_settings_type);

ShieldsSettingCounts GetFPSettingCountFromRules(
    const ContentSettingsForOneType& fp_rules);
ShieldsSettingCounts GetAdsSettingCountFromRules(
    const ContentSettingsForOneType& ads_rules);
}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_UTILS_H_
