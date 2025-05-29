// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_SERVICE_HELPER_H_

#include <optional>
#include <string>

namespace wootz_adblock {

void MergeCspDirectiveInto(std::optional<std::string> from,
                           std::optional<std::string>* into);

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CORE_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
