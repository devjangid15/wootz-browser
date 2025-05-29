/* Copyright (c) 2025 The WootzApp Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CHROME_BROWSER_WOOTZ_ADBLOCK_AD_BLOCK_PREF_SERVICE_FACTORY_H_
#define CHROME_BROWSER_WOOTZ_ADBLOCK_AD_BLOCK_PREF_SERVICE_FACTORY_H_

#include <memory>

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace wootz_adblock {

class AdBlockPrefService;

class AdBlockPrefServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  AdBlockPrefServiceFactory(const AdBlockPrefServiceFactory&) = delete;
  AdBlockPrefServiceFactory& operator=(const AdBlockPrefServiceFactory&) =
      delete;

  static AdBlockPrefService* GetForBrowserContext(
      content::BrowserContext* context);

  static AdBlockPrefServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<AdBlockPrefServiceFactory>;

  AdBlockPrefServiceFactory();
  ~AdBlockPrefServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext(
      content::BrowserContext* context) const override;

  // We use the same service in both normal and incognito modes.
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  bool ServiceIsCreatedWithBrowserContext() const override;
};

}  // namespace wootz_adblock

#endif  // CHROME_BROWSER_WOOTZ_ADBLOCK_AD_BLOCK_PREF_SERVICE_FACTORY_H_
