// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_DOMAIN_BLOCK_NAVIGATION_THROTTLE_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_DOMAIN_BLOCK_NAVIGATION_THROTTLE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/wootz_adblock/content/browser/wootz_adblock_util.h"
#include "content/public/browser/navigation_throttle.h"
#include "url/gurl.h"

class HostContentSettingsMap;

namespace content {
class NavigationHandle;
}  // namespace content

namespace ephemeral_storage {
class EphemeralStorageService;
}  // namespace ephemeral_storage

namespace wootz_adblock {

class AdBlockService;
class AdBlockCustomFiltersProvider;

class DomainBlockNavigationThrottle : public content::NavigationThrottle {
 public:
  struct BlockResult;
  explicit DomainBlockNavigationThrottle(
      content::NavigationHandle* navigation_handle,
      AdBlockService* ad_block_service,
      AdBlockCustomFiltersProvider* ad_block_custom_filters_provider,
      ephemeral_storage::EphemeralStorageService* ephemeral_storage_service,
      HostContentSettingsMap* content_settings,
      const std::string& locale);
  ~DomainBlockNavigationThrottle() override;

  DomainBlockNavigationThrottle(const DomainBlockNavigationThrottle&) = delete;
  DomainBlockNavigationThrottle& operator=(
      const DomainBlockNavigationThrottle&) = delete;

  static std::unique_ptr<DomainBlockNavigationThrottle> MaybeCreateThrottleFor(
      content::NavigationHandle* navigation_handle,
      AdBlockService* ad_block_service,
      AdBlockCustomFiltersProvider* ad_block_custom_filters_provider,
      ephemeral_storage::EphemeralStorageService* ephemeral_storage_service,
      HostContentSettingsMap* content_settings,
      const std::string& locale);

  // content::NavigationThrottle implementation:
  content::NavigationThrottle::ThrottleCheckResult WillStartRequest() override;
  content::NavigationThrottle::ThrottleCheckResult WillRedirectRequest()
      override;
  content::NavigationThrottle::ThrottleCheckResult WillProcessResponse()
      override;
  const char* GetNameForLogging() override;

 private:
  void OnShouldBlockDomain(DomainBlockingType domain_blocking_type,
                           const BlockResult& should_block_domain);
  void ShowInterstitial(bool proceed_with_resume_cancel);
  void Enable1PESAndResume(bool proceed_with_resume_cancel);
  void On1PESState(bool proceed_with_resume_cancel, bool is_1pes_enabled);
  void RestartNavigation(const GURL& url, bool proceed_with_resume_cancel);

  const raw_ptr<AdBlockService> ad_block_service_ = nullptr;
  const raw_ptr<AdBlockCustomFiltersProvider>
      ad_block_custom_filters_provider_ = nullptr;
  const raw_ptr<ephemeral_storage::EphemeralStorageService>
      ephemeral_storage_service_ = nullptr;
  const raw_ptr<HostContentSettingsMap> content_settings_ = nullptr;
  std::string locale_;
  bool is_deferred_ = false;

  base::WeakPtrFactory<DomainBlockNavigationThrottle> weak_ptr_factory_{this};
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_DOMAIN_BLOCK_NAVIGATION_THROTTLE_H_
