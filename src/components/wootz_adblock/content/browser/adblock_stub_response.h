// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_ADBLOCK_STUB_RESPONSE_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_ADBLOCK_STUB_RESPONSE_H_

#include <optional>
#include <string>

#include "services/network/public/mojom/url_response_head.mojom-forward.h"

namespace network {
struct ResourceRequest;
}  // namespace network

namespace wootz_adblock {

// Intercepts certain requests and blocks them by silently returning 200 OK
// and not allowing them to hit the network.
void MakeStubResponse(const std::optional<std::string>& data_url,
                      const network::ResourceRequest& request,
                      network::mojom::URLResponseHeadPtr* response,
                      std::string* data);

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_ADBLOCK_STUB_RESPONSE_H_
