// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_PREF_NAMES_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_PREF_NAMES_H_

namespace wootz_adblock {
namespace prefs {

inline constexpr char kAdBlockCheckedDefaultRegion[] =
    "wootz.ad_block.checked_default_region";
inline constexpr char kAdBlockCheckedAllDefaultRegions[] =
    "wootz.ad_block.checked_all_default_regions";
inline constexpr char kAdBlockCookieListOptInShown[] =
    "wootz.ad_block.cookie_list_opt_in_shown";
inline constexpr char kAdBlockCookieListSettingTouched[] =
    "wootz.ad_block.cookie_list_setting_touched";
inline constexpr char kAdBlockMobileNotificationsListSettingTouched[] =
    "wootz.ad_block.mobile_notifications_list_setting_touched";

inline constexpr char kAdBlockCustomFilters[] = "wootz.ad_block.custom_filters";
inline constexpr char kAdBlockRegionalFilters[] =
    "wootz.ad_block.regional_filters";
inline constexpr char kAdBlockListSubscriptions[] =
    "wootz.ad_block.list_subscriptions";
inline constexpr char kAdBlockDeveloperMode[] = "wootz.ad_block.developer_mode";

inline constexpr char kFBEmbedControlType[] = "wootz.fb_embed_default";
inline constexpr char kTwitterEmbedControlType[] =
    "wootz.twitter_embed_default";
inline constexpr char kLinkedInEmbedControlType[] =
    "wootz.linkedin_embed_default";
inline constexpr char kReduceLanguageEnabled[] = "wootz.reduce_language";

}  // namespace prefs
}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_COMMON_PREF_NAMES_H_
