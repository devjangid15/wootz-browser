/* Copyright (c) 2025 The WootzApp Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CHROME_BROWSER_WOOTZ_ADBLOCK_WOOTZ_ADBLOCK_WEB_CONTENTS_OBSERVER_H_
#define CHROME_BROWSER_WOOTZ_ADBLOCK_WOOTZ_ADBLOCK_WEB_CONTENTS_OBSERVER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/synchronization/lock.h"
#include "components/wootz_adblock/core/common/wootz_adblock.mojom.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/browser/render_frame_host_receiver_set.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class PrefRegistrySimple;

namespace wootz_adblock {

class BraveShieldsWebContentsObserver
    : public content::WebContentsObserver,
      public content::WebContentsUserData<BraveShieldsWebContentsObserver>,
      public wootz_adblock::mojom::BraveShieldsHost {
 public:
  explicit BraveShieldsWebContentsObserver(content::WebContents*);
  BraveShieldsWebContentsObserver(const BraveShieldsWebContentsObserver&) =
      delete;
  BraveShieldsWebContentsObserver& operator=(
      const BraveShieldsWebContentsObserver&) = delete;
  ~BraveShieldsWebContentsObserver() override;

  static void BindBraveShieldsHost(
      mojo::PendingAssociatedReceiver<wootz_adblock::mojom::BraveShieldsHost>
          receiver,
      content::RenderFrameHost* rfh);

  static void RegisterProfilePrefs(PrefRegistrySimple* registry);
  static void DispatchBlockedEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchAllowedOnceEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchWebcompatFeatureInvokedForWebContents(
      ContentSettingsType webcompat_content_settings,
      content::WebContents* web_contents);
  static void DispatchBlockedEvent(const GURL& request_url,
                                   content::FrameTreeNodeId frame_tree_node_id,
                                   const std::string& block_type);
  static GURL GetTabURLFromRenderFrameInfo(int render_frame_tree_node_id);
  void AllowScriptsOnce(const std::vector<std::string>& origins);
  void BlockAllowedScripts(const std::vector<std::string>& origins);
  bool IsBlockedSubresource(const std::string& subresource);
  void AddBlockedSubresource(const std::string& subresource);

 protected:
  // content::WebContentsObserver overrides.
  void ReadyToCommitNavigation(
      content::NavigationHandle* navigation_handle) override;

  // wootz_adblock::mojom::BraveShieldsHost.
  void OnJavaScriptBlocked(const std::u16string& details) override;
  void OnJavaScriptAllowedOnce(const std::u16string& details) override;
  void OnWebcompatFeatureInvoked(
      ContentSettingsType webcompat_settings_type) override;

 private:
  friend class content::WebContentsUserData<BraveShieldsWebContentsObserver>;
  friend class BraveShieldsWebContentsObserverBrowserTest;

  // Allows indicating a implementor of wootz_adblock::mojom::BraveShieldsHost
  // other than this own class, for testing purposes only.
  static void SetReceiverImplForTesting(BraveShieldsWebContentsObserver* impl);

  // Only used from the BindBraveShieldsHost() static method, useful to bind
  // the mojo receiver of wootz_adblock::mojom::BraveShieldsHost to a
  // different implementor when needed, for testing purposes.
  void BindReceiver(mojo::PendingAssociatedReceiver<
                        wootz_adblock::mojom::BraveShieldsHost> receiver,
                    content::RenderFrameHost* rfh);

  // Sends the current shields settings to the renderer process bound to the
  // given |navigation_handle|.
  void SendShieldsSettings(content::NavigationHandle* navigation_handle);

  std::vector<std::string> allowed_scripts_;
  // We keep a set of the current page's blocked URLs in case the page
  // continually tries to load the same blocked URLs.
  std::set<std::string> blocked_url_paths_;

  content::RenderFrameHostReceiverSet<wootz_adblock::mojom::BraveShieldsHost>
      receivers_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace wootz_adblock

#endif  // CHROME_BROWSER_WOOTZ_ADBLOCK_WOOTZ_ADBLOCK_WEB_CONTENTS_OBSERVER_H_
