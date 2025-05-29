// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_LOCALIZED_STRINGS_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_LOCALIZED_STRINGS_H_

#include "components/grit/wootz_adblock_components_strings.h"
#include "ui/base/webui/web_ui_util.h"

namespace wootz_adblock {

inline constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"wootzAdblock", IDS_WOOTZ_ADBLOCK},
    {"wootzAdblockStandalone", IDS_WOOTZ_ADBLOCK_STANDALONE},
    {"wootzAdblockEnable", IDS_WOOTZ_ADBLOCK_ENABLE},
    {"wootzAdblockUp", IDS_WOOTZ_ADBLOCK_UP},
    {"wootzAdblockDown", IDS_WOOTZ_ADBLOCK_DOWN},
    {"wootzAdblockBroken", IDS_WOOTZ_ADBLOCK_BROKEN},
    {"wootzAdblockBlockedNote", IDS_WOOTZ_ADBLOCK_BLOCKED_NOTE},
    {"wootzAdblockNOTBlockedNote", IDS_WOOTZ_ADBLOCK_NOT_BLOCKED_NOTE},
    {"wootzAdblockAdvancedCtrls", IDS_WOOTZ_ADBLOCK_ADVANCED_CTRLS},
    {"braveShieldSettingsDescription", IDS_BRAVE_SHIELD_SETTINGS_DESCRIPTION},
    {"wootzAdblockGlobalSettingsTitle",
     IDS_WOOTZ_ADBLOCK_GLOBAL_SETTINGS_TITLE},
    {"wootzAdblockChangeDefaults", IDS_WOOTZ_ADBLOCK_CHANGE_DEFAULTS},
    {"wootzAdblockCustomizeAdblockLists",
     IDS_WOOTZ_ADBLOCK_CUSTOMIZE_ADBLOCK_LISTS},
    {"wootzAdblockConnectionsUpgraded", IDS_WOOTZ_ADBLOCK_CONNECTIONS_UPGRADED},
    {"wootzAdblockHTTPSEnable", IDS_WOOTZ_ADBLOCK_HTTPS_ENABLE},
    {"wootzAdblockScriptsBlocked", IDS_WOOTZ_ADBLOCK_SCRIPTS_BLOCKED},
    {"wootzAdblockScriptsBlockedEnable",
     IDS_WOOTZ_ADBLOCK_SCRIPTS_BLOCKED_ENABLE},
    {"wootzAdblockTrackersAndAds", IDS_WOOTZ_ADBLOCK_TRACKERS_AND_ADS},
    {"wootzAdblockTrackersAndAdsBlockedStd",
     IDS_WOOTZ_ADBLOCK_TRACKERS_AND_ADS_BLOCKED_STD},
    {"wootzAdblockTrackersAndAdsBlockedAgg",
     IDS_WOOTZ_ADBLOCK_TRACKERS_AND_ADS_BLOCKED_AGG},
    {"wootzAdblockTrackersAndAdsAllowAll",
     IDS_WOOTZ_ADBLOCK_TRACKERS_AND_ADS_ALLOW_ALL},
    {"wootzAdblockThirdPartyCookiesBlocked",
     IDS_WOOTZ_ADBLOCK_THIRD_PARTY_COOKIES_BLOCKED},
    {"wootzAdblockForgetFirstPartyStorage",
     IDS_WOOTZ_ADBLOCK_FORGET_FIRST_PARTY_STORAGE_LABEL},
    {"wootzAdblockCookiesBlockAll", IDS_WOOTZ_ADBLOCK_COOKIES_BLOCKED},
    {"wootzAdblockCookiesAllowedAll", IDS_WOOTZ_ADBLOCK_COOKIES_ALLOWED_ALL},
    {"wootzAdblockFingerprintingBlocked",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_BLOCKED},
    {"wootzAdblockFingerprintingBlockedStd",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_BLOCKED_STD},
    {"wootzAdblockFingerprintingBlockedAgg",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_BLOCKED_AGG},
    {"wootzAdblockFingerprintingAllowAll",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_ALLOW_ALL},
    {"wootzAdblockHttpsUpgradeModeDisabled",
     IDS_WOOTZ_ADBLOCK_HTTPS_UPGRADE_MODE_DISABLED},
    {"wootzAdblockHttpsUpgradeModeStandard",
     IDS_WOOTZ_ADBLOCK_HTTPS_UPGRADE_MODE_STANDARD},
    {"wootzAdblockHttpsUpgradeModeStrict",
     IDS_WOOTZ_ADBLOCK_HTTPS_UPGRADE_MODE_STRICT},
    {"wootzAdblockReportSite", IDS_WOOTZ_ADBLOCK_REPORT_SITE},
    {"wootzAdblockReportSiteDesc", IDS_WOOTZ_ADBLOCK_REPORT_SITE_DESC},
    {"wootzAdblockDownDesc", IDS_WOOTZ_ADBLOCK_DOWN_DESC},
    {"wootzAdblockBlockedScriptsLabel",
     IDS_WOOTZ_ADBLOCK_BLOCKED_SCRIPTS_LABEL},
    {"wootzAdblockFingerprintingProtectionsAppliedLabel",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_PROTECTIONS_APPLIED_LABEL},
    {"wootzAdblockLearnMoreLinkText", IDS_WOOTZ_ADBLOCK_LEARN_MORE_LINK_TEXT},
    {"wootzAdblockFingerprintingListDescription",
     IDS_WOOTZ_ADBLOCK_FINGERPRINTING_LIST_DESCRIPTION},
    {"wootzAdblockAllowedScriptsLabel",
     IDS_WOOTZ_ADBLOCK_ALLOWED_SCRIPTS_LABEL},
    {"wootzAdblockBlockedFingerprintsLabel",
     IDS_WOOTZ_ADBLOCK_BLOCKED_FINGERPRINTS_LABEL},
    {"wootzAdblockAllowedFingerprintsLabel",
     IDS_WOOTZ_ADBLOCK_ALLOWED_FINGERPRINTS_LABEL},
    {"wootzAdblockManaged", IDS_WOOTZ_ADBLOCK_MANAGED},
    {"wootzAdblockAllowScriptOnce", IDS_WOOTZ_ADBLOCK_ALLOW_SCRIPT_ONCE},
    {"wootzAdblockBlockScript", IDS_WOOTZ_ADBLOCK_SCRIPT_BLOCK},
    {"wootzAdblockAllowScriptsAll", IDS_WOOTZ_ADBLOCK_ALLOW_SCRIPTS_ALL},
    {"wootzAdblockBlockScriptsAll", IDS_WOOTZ_ADBLOCK_BLOCK_SCRIPTS_ALL}};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_WOOTZ_ADBLOCK_LOCALIZED_STRINGS_H_
