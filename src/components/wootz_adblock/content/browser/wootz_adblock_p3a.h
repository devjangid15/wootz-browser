// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_wootz_adblock_P3A_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_wootz_adblock_P3A_H_

#include "components/wootz_adblock/content/browser/wootz_adblock_util.h"

class PrefRegistrySimple;
class PrefService;
class HostContentSettingsMap;

namespace wootz_adblock {

inline constexpr char kUsagePrefName[] = "wootz_adblock.p3a_usage";
inline constexpr char kFirstReportedPrefName[] =
    "wootz_adblock.p3a_first_reported_v2";  // DEPRECATED
inline constexpr char kFirstReportedRevisionPrefName[] =
    "wootz_adblock.p3a_first_reported_revision";

inline constexpr char kAdsStrictCountPrefName[] =
    "wootz_adblock.p3a_ads_strict_domain_count";
inline constexpr char kAdsStandardCountPrefName[] =
    "wootz_adblock.p3a_ads_standard_domain_count";
inline constexpr char kAdsAllowCountPrefName[] =
    "wootz_adblock.p3a_ads_allow_domain_count";
inline constexpr char kFPStrictCountPrefName[] =
    "wootz_adblock.p3a_fp_strict_domain_count";
inline constexpr char kFPStandardCountPrefName[] =
    "wootz_adblock.p3a_fp_standard_domain_count";
inline constexpr char kFPAllowCountPrefName[] =
    "wootz_adblock.p3a_fp_allow_domain_count";

inline constexpr char kAdsSettingHistogramName[] =
    "Wootz.AdBlock.AdBlockSetting";
inline constexpr char kFingerprintSettingHistogramName[] =
    "Wootz.AdBlock.FingerprintBlockSetting";
inline constexpr char kUsageStatusHistogramName[] = "Wootz.AdBlock.UsageStatus";
inline constexpr char kDomainAdsSettingsAboveHistogramName[] =
    "Wootz.AdBlock.DomainAdsSettingsAboveGlobal";
inline constexpr char kDomainAdsSettingsBelowHistogramName[] =
    "Wootz.AdBlock.DomainAdsSettingsBelowGlobal";
inline constexpr char kDomainFPSettingsAboveHistogramName[] =
    "Wootz.AdBlock.DomainFingerprintSettingsAboveGlobal";
inline constexpr char kDomainFPSettingsBelowHistogramName[] =
    "Wootz.AdBlock.DomainFingerprintSettingsBelowGlobal";
inline constexpr char kForgetFirstPartyHistogramName[] =
    "Wootz.AdBlock.ForgetFirstParty";
// Note: append-only enumeration! Never remove any existing values, as this enum
// is used to bucket a UMA histogram, and removing values breaks that.
enum ShieldsIconUsage {
  kNeverClicked,
  kClicked,
  kShutOffShields,
  kChangedPerSiteShields,
  kSize,
};

// We save latest value to local state and compare new values with it.
// The idea is to write to a histogram only the highest value (e.g. we are
// not interested in |kClicked| event if the user already turned off shields.
// Sine P3A sends only latest written values, these is enough for our current
// goals.
void MaybeRecordShieldsUsageP3A(ShieldsIconUsage usage,
                                PrefService* local_state);

// Records to global ads setting histogram: Wootz.AdBlock.AdBlockSetting
void RecordShieldsAdsSetting(ControlType setting);

// Records to global FP setting histogram: Wootz.AdBlock.FingerprintBlockSetting
void RecordShieldsFingerprintSetting(ControlType setting);

// To be called when the global setting changes.
// Will update domain setting count histograms.
void RecordShieldsDomainSettingCounts(PrefService* profile_prefs,
                                      bool is_fingerprint,
                                      ControlType global_setting);

// To be called when a domain setting changes.
// Will update internal pref counts and update domain setting count histograms.
void RecordShieldsDomainSettingCountsWithChange(PrefService* profile_prefs,
                                                bool is_fingerprint,
                                                ControlType global_setting,
                                                ControlType* prev_setting,
                                                ControlType new_setting);

// Records global "forget me when I close this site" setting,
// and any per-site exceptions.
void RecordForgetFirstPartySetting(HostContentSettingsMap* map);

void RegisterShieldsP3ALocalPrefs(PrefRegistrySimple* local_state);

void RegisterShieldsP3AProfilePrefs(PrefRegistrySimple* registry);
void RegisterShieldsP3AProfilePrefsForMigration(PrefRegistrySimple* registry);
void MigrateObsoleteProfilePrefs(PrefService* profile_prefs);

// To be called at initialization. Will count all domain settings and
// record to all histograms, if executed for the first time.
void MaybeRecordInitialShieldsSettings(PrefService* profile_prefs,
                                       HostContentSettingsMap* map);

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_wootz_adblock_P3A_H_
