/* Copyright (c) 2025 The WootzApp Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef COMPONENTS_WOOTZ_ADBLOCK_ADBLOCK_RESOLVER_ADBLOCK_DOMAIN_RESOLVER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_ADBLOCK_RESOLVER_ADBLOCK_DOMAIN_RESOLVER_H_

#include <stdint.h>
#include <string>

namespace adblock {

struct DomainPosition;

DomainPosition resolve_domain_position(const std::string& host);

}  // namespace adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_ADBLOCK_RESOLVER_ADBLOCK_DOMAIN_RESOLVER_H_
