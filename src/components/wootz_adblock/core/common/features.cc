// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/core/common/features.h"

#include "base/feature_list.h"

namespace wootz_adblock::features {

BASE_FEATURE(kAdBlockDefaultResourceUpdateInterval,
             "AdBlockDefaultResourceUpdateInterval",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will block first-party requests that appear in a filter
// list when Shields is in "standard" blocking mode. When disabled, Wootz will
// allow first-party requests in "standard" blocking mode regardless of whether
// or not they appear in a filter list.
BASE_FEATURE(kWootzAdblockDefault1pBlocking,
             "WootzAdblockDefault1pBlocking",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Wootz will issue DNS queries for requests that the adblock
// engine has not blocked, then check them again with the original hostname
// substituted for any canonical name found.
BASE_FEATURE(kWootzAdblockCnameUncloaking,
             "WootzAdblockCnameUncloaking",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will apply HTML element collapsing to all images and
// iframes that initiate a blocked network request.
BASE_FEATURE(kWootzAdblockCollapseBlockedElements,
             "WootzAdblockCollapseBlockedElements",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will enable "Easylist-Cookie List" by default unless
// overridden by a locally set preference.
BASE_FEATURE(kWootzAdblockCookieListDefault,
             "WootzAdblockCookieListDefault",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will display a bubble inviting the user to turn on the
// "Easylist-Cookie List" filter.
BASE_FEATURE(kWootzAdblockCookieListOptIn,
             "WootzAdblockCookieListOptIn",
             base::FEATURE_DISABLED_BY_DEFAULT);
BASE_FEATURE(kWootzAdblockCosmeticFiltering,
             "WootzAdblockCosmeticFiltering",
             base::FEATURE_ENABLED_BY_DEFAULT);
// Wootz will apply cosmetic filters with procedural operators like
// `:has-text(...)` and `:upward(...)`.
BASE_FEATURE(kWootzAdblockProceduralFiltering,
             "WootzAdblockProceduralFiltering",
             base::FEATURE_ENABLED_BY_DEFAULT);
BASE_FEATURE(kWootzAdblockScriptletDebugLogs,
             "WootzAdblockScriptletDebugLogs",
             base::FEATURE_DISABLED_BY_DEFAULT);
BASE_FEATURE(kWootzAdblockCspRules,
             "WootzAdblockCspRules",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will enable "Fanboy's Mobile Notifications List" by
// default unless overridden by a locally set preference.
BASE_FEATURE(kWootzAdblockMobileNotificationsListDefault,
             "WootzAdblockMobileNotificationsListDefault",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will enable "Wootz Experimental Adblock Rules" list by
// default unless overridden by a locally set preference.
// NOTE: this should only be turned on by default in Nightly and Beta.
BASE_FEATURE(kWootzAdblockExperimentalListDefault,
             "WootzAdblockExperimentalListDefault",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Wootz will block domains listed in the user's selected adblock
// filters and present a security interstitial with choice to proceed and
// optionally whitelist the domain.
// Domain block filters look like this:
// ||ads.example.com^
BASE_FEATURE(kWootzDomainBlock,
             "WootzDomainBlock",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Wootz will attempt to enable 1PES mode in a standard blocking
// mode when a user visists a domain that is present in currently active adblock
// filters. 1PES will be enabled only if neither cookies nor localStorage data
// is stored for the website.
BASE_FEATURE(kWootzDomainBlock1PES,
             "WootzDomainBlock1PES",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, network requests initiated by extensions will be checked and
// potentially blocked by Wootz Shields.
BASE_FEATURE(kWootzExtensionNetworkBlocking,
             "WootzExtensionNetworkBlocking",
             base::FEATURE_DISABLED_BY_DEFAULT);
// Enables Wootz farbling (randomization of fingerprinting-susceptible WebAPIs).
BASE_FEATURE(kWootzFarbling, "WootzFarbling", base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, language headers and APIs may be altered by Wootz Shields.
BASE_FEATURE(kWootzReduceLanguage,
             "WootzReduceLanguage",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, wootz shred feature will be available
BASE_FEATURE(kWootzShredFeature,
             "WootzShredFeature",
#if BUILDFLAG(IS_IOS)
             base::FEATURE_ENABLED_BY_DEFAULT);
#else
             base::FEATURE_DISABLED_BY_DEFAULT);
#endif
// When enabled, wootz shred will clear all cache data when shredding.
BASE_FEATURE(kWootzShredCacheData,
             "WootzShredCacheData",
#if BUILDFLAG(IS_IOS)
             base::FEATURE_ENABLED_BY_DEFAULT);
#else
             base::FEATURE_DISABLED_BY_DEFAULT);
#endif
// When enabled, will display debug menu for adblock features in the Shields
// panel.
BASE_FEATURE(kWootzIOSDebugAdblock,
             "WootzIOSDebugAdblock",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, show Strict (aggressive) fingerprinting mode in Wootz Shields.
BASE_FEATURE(kWootzShowStrictFingerprintingMode,
             "WootzShowStrictFingerprintingMode",
             base::FEATURE_DISABLED_BY_DEFAULT);
// when enabled, wootz will prompt for permission on sites which want to connect
// to localhost.
BASE_FEATURE(kWootzLocalhostAccessPermission,
             "WootzLocalhostAccessPermission",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Wootz will always report Light in Fingerprinting: Strict mode
BASE_FEATURE(kWootzDarkModeBlock,
             "WootzDarkModeBlock",
             base::FEATURE_ENABLED_BY_DEFAULT);
// load the cosmetic filter rules using sync ipc
BASE_FEATURE(kCosmeticFilteringSyncLoad,
             "CosmeticFilterSyncLoad",
             base::FEATURE_ENABLED_BY_DEFAULT);
// If the feature flag is on, we show the Block all Cookies toggle
BASE_FEATURE(kBlockAllCookiesToggle,
             "BlockAllCookiesToggle",
             base::FEATURE_DISABLED_BY_DEFAULT);
// when enabled, allow to select and block HTML elements
BASE_FEATURE(kWootzAdblockElementPicker,
             "WootzAdblockElementPicker",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Enables extra TRACE_EVENTs in content filter js. The feature is
// primary designed for local debugging.
BASE_FEATURE(kCosmeticFilteringExtraPerfMetrics,
             "CosmeticFilteringExtraPerfMetrics",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kCosmeticFilteringJsPerformance,
             "CosmeticFilteringJsPerformance",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kCosmeticFilteringCustomScriptlets,
             "CosmeticFilteringCustomScriptlets",
             base::FEATURE_DISABLED_BY_DEFAULT);

constexpr base::FeatureParam<int> kComponentUpdateCheckIntervalMins{
    &kAdBlockDefaultResourceUpdateInterval, "update_interval_mins", 100};

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringSubFrameFirstSelectorsPollingDelayMs{
        &kCosmeticFilteringJsPerformance, "subframes_first_query_delay_ms",
        "1000"};

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringswitchToSelectorsPollingThreshold{
        &kCosmeticFilteringJsPerformance, "switch_to_polling_threshold",
        "1000"};

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringFetchNewClassIdRulesThrottlingMs{
        &kCosmeticFilteringJsPerformance, "fetch_throttling_ms", "100"};

BASE_FEATURE(kAdblockOverrideRegexDiscardPolicy,
             "AdblockOverrideRegexDiscardPolicy",
             base::FEATURE_DISABLED_BY_DEFAULT);

constexpr base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyCleanupIntervalSec{
        &kAdblockOverrideRegexDiscardPolicy, "cleanup_interval_sec", 0};

constexpr base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyDiscardUnusedSec{
        &kAdblockOverrideRegexDiscardPolicy, "discard_unused_sec", 180};

}  // namespace wootz_adblock::features
