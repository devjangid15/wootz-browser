// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_DEFAULT_PROMO_GENERIC_DEFAULT_BROWSER_GENERIC_PROMO_COORDINATOR_H_
#define IOS_CHROME_BROWSER_UI_DEFAULT_PROMO_GENERIC_DEFAULT_BROWSER_GENERIC_PROMO_COORDINATOR_H_

#import "ios/chrome/browser/shared/coordinator/chrome_coordinator/chrome_coordinator.h"
#import "ios/chrome/browser/ui/default_promo/generic/default_browser_generic_promo_commands.h"

@protocol PromosManagerUIHandler;

@interface DefaultBrowserGenericPromoCoordinator : ChromeCoordinator

// Handler for all actions of this coordinator.
@property(nonatomic, weak) id<DefaultBrowserGenericPromoCommands> handler;

// Whether or not the current showing came from a past Remind Me Later.
@property(nonatomic, assign) BOOL promoWasFromRemindMeLater;

// The promos manager ui handler to alert for promo UI changes. Should only be
// set if this coordinator was a promo presented by the PromosManager.
@property(nonatomic, weak) id<PromosManagerUIHandler> promosUIHandler;

@end

#endif  // IOS_CHROME_BROWSER_UI_DEFAULT_PROMO_GENERIC_DEFAULT_BROWSER_GENERIC_PROMO_COORDINATOR_H_
