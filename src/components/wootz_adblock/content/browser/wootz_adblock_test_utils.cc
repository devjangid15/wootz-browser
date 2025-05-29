// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/wootz_adblock/content/browser/wootz_adblock_test_utils.h"

namespace wootz_adblock {

// Declared in wootz_adblock_util.cc.
extern uint32_t g_stable_farbling_tokens_seed;

ScopedStableFarblingTokensForTesting::ScopedStableFarblingTokensForTesting(
    uint32_t seed)
    : scoped_stable_farbling_token_seed_(&g_stable_farbling_tokens_seed, seed) {
}

ScopedStableFarblingTokensForTesting::~ScopedStableFarblingTokensForTesting() =
    default;

}  // namespace wootz_adblock
