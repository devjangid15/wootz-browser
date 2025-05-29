// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_FEATURES_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_FEATURES_H_

#include <string>

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace wootz_adblock {
namespace features {
BASE_DECLARE_FEATURE(kAdBlockDefaultResourceUpdateInterval);
BASE_DECLARE_FEATURE(kWootzAdblockCnameUncloaking);
BASE_DECLARE_FEATURE(kWootzAdblockCollapseBlockedElements);
BASE_DECLARE_FEATURE(kWootzAdblockCookieListDefault);
BASE_DECLARE_FEATURE(kWootzAdblockCookieListOptIn);
BASE_DECLARE_FEATURE(kWootzAdblockCosmeticFiltering);
BASE_DECLARE_FEATURE(kWootzAdblockProceduralFiltering);
BASE_DECLARE_FEATURE(kWootzAdblockCspRules);
BASE_DECLARE_FEATURE(kWootzAdblockDefault1pBlocking);
BASE_DECLARE_FEATURE(kWootzAdblockMobileNotificationsListDefault);
BASE_DECLARE_FEATURE(kWootzAdblockExperimentalListDefault);
BASE_DECLARE_FEATURE(kWootzAdblockScriptletDebugLogs);
BASE_DECLARE_FEATURE(kWootzDarkModeBlock);
BASE_DECLARE_FEATURE(kWootzDomainBlock);
BASE_DECLARE_FEATURE(kWootzDomainBlock1PES);
BASE_DECLARE_FEATURE(kWootzExtensionNetworkBlocking);
BASE_DECLARE_FEATURE(kWootzFarbling);
BASE_DECLARE_FEATURE(kWootzLocalhostAccessPermission);
BASE_DECLARE_FEATURE(kWootzReduceLanguage);
BASE_DECLARE_FEATURE(kWootzShredFeature);
BASE_DECLARE_FEATURE(kWootzShredCacheData);
BASE_DECLARE_FEATURE(kWootzIOSDebugAdblock);
BASE_DECLARE_FEATURE(kWootzShowStrictFingerprintingMode);
BASE_DECLARE_FEATURE(kCosmeticFilteringExtraPerfMetrics);
BASE_DECLARE_FEATURE(kCosmeticFilteringJsPerformance);
BASE_DECLARE_FEATURE(kCosmeticFilteringSyncLoad);
BASE_DECLARE_FEATURE(kBlockAllCookiesToggle);
BASE_DECLARE_FEATURE(kCosmeticFilteringCustomScriptlets);
BASE_DECLARE_FEATURE(kWootzAdblockElementPicker);
extern const base::FeatureParam<int> kComponentUpdateCheckIntervalMins;
extern const base::FeatureParam<std::string>
    kCosmeticFilteringSubFrameFirstSelectorsPollingDelayMs;
extern const base::FeatureParam<std::string>
    kCosmeticFilteringswitchToSelectorsPollingThreshold;
extern const base::FeatureParam<std::string>
    kCosmeticFilteringFetchNewClassIdRulesThrottlingMs;
BASE_DECLARE_FEATURE(kAdblockOverrideRegexDiscardPolicy);
extern const base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyCleanupIntervalSec;
extern const base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyDiscardUnusedSec;

}  // namespace features
}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_FEATURES_H_
