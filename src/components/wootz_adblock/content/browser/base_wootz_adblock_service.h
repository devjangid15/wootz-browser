// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_BASE_wootz_adblock_SERVICE_H_
#define COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_BASE_wootz_adblock_SERVICE_H_

#include <stdint.h>

#include <string>
// TODO(brave): <mutex> is an unapproved C++11 header
#include <mutex>  // NOLINT

#include "base/task/sequenced_task_runner.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

namespace wootz_adblock {

// The wootz adblock service in charge of checking wootz adblock like ad-block,
// tracking protection, etc.
class BaseWootzAdblockService {
 public:
  explicit BaseWootzAdblockService(
      scoped_refptr<base::SequencedTaskRunner> task_runner);
  virtual ~BaseWootzAdblockService();
  bool Start();
  bool IsInitialized() const;
  virtual void ShouldStartRequest(const GURL& url,
                                  blink::mojom::ResourceType resource_type,
                                  const std::string& tab_host,
                                  bool aggressive_blocking,
                                  bool* did_match_rule,
                                  bool* did_match_exception,
                                  bool* did_match_important,
                                  std::string* mock_data_url);

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner();

 protected:
  virtual bool Init() = 0;

 private:
  void InitShields();

  bool initialized_;
  std::mutex initialized_mutex_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
};

}  // namespace wootz_adblock

#endif  // COMPONENTS_WOOTZ_ADBLOCK_CONTENT_BROWSER_BASE_wootz_adblock_SERVICE_H_
