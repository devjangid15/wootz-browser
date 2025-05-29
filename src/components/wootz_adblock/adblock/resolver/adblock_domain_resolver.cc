/* Copyright (c) 2025 The WootzApp Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/wootz_adblock/adblock/resolver/adblock_domain_resolver.h"

#include <string>

#include "components/wootz_adblock/adblock/rs/src/lib.rs.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"

namespace adblock {

// Extracts the start and end characters of a domain from a hostname.
// Required for correct functionality of adblock-rust.
DomainPosition resolve_domain_position(const std::string& host) {
  const auto domain = net::registry_controlled_domains::GetDomainAndRegistry(
      host, net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
  const size_t match = host.rfind(domain);
  DomainPosition position;
  if (match != std::string::npos) {
    position.start = match;
    position.end = match + domain.length();
  } else {
    position.start = 0;
    position.end = host.length();
  }
  return position;
}

}  // namespace adblock
