// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'chrome-untrusted://lens/lens_overlay_app.js';

import {BrowserProxyImpl} from 'chrome-untrusted://lens/browser_proxy.js';
import type {LensOverlayAppElement} from 'chrome-untrusted://lens/lens_overlay_app.js';
import {waitBeforeNextRender} from 'chrome-untrusted://webui-test/polymer_test_util.js';

import {TestLensOverlayBrowserProxy} from './test_overlay_browser_proxy.js';

suite('OverlayEscapeKey', () => {
  let testBrowserProxy: TestLensOverlayBrowserProxy;
  let lensOverlayElement: LensOverlayAppElement;

  setup(() => {
    // Resetting the HTML needs to be the first thing we do in setup to
    // guarantee that any singleton instances don't change while any UI is still
    // attached to the DOM.
    document.body.innerHTML = window.trustedTypes!.emptyHTML;

    testBrowserProxy = new TestLensOverlayBrowserProxy();
    BrowserProxyImpl.setInstance(testBrowserProxy);

    lensOverlayElement = document.createElement('lens-overlay-app');
    document.body.appendChild(lensOverlayElement);
    return waitBeforeNextRender(lensOverlayElement);
  });

  test('EscapeKeyPressClosesOverlay', () => {
    lensOverlayElement.dispatchEvent(
        new KeyboardEvent('keyup', {bubbles: true, key: 'Escape'}));
    return testBrowserProxy.handler.whenCalled(
        'closeRequestedByOverlayEscapeKeyPress');
  });
});
