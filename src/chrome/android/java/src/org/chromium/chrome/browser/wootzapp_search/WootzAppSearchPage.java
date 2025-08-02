// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.wootzapp_search;

import android.app.Activity;
import android.view.View;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.ui.native_page.BasicNativePage;
import org.chromium.chrome.browser.ui.native_page.NativePageHost;
import org.chromium.components.browser_ui.bottomsheet.BottomSheetController;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.tab.Tab;

/** Native page for search functionality. */
public class WootzAppSearchPage extends BasicNativePage {
    private WootzAppSearchManager mWootzAppSearchManager;
    private String mTitle;

    /**
     * Create a new instance of the search page.
     *
     * @param activity The {@link Activity} used to get context and instantiate the {@link
     *     WootzAppSearchManager}.
     * @param host A NativePageHost to load URLs.
     * @param snackbarManager The {@link SnackbarManager} used to display snackbars.
     * @param profile The Profile of the current tab.
     * @param bottomSheetController {@link BottomSheetController} object.
     * @param tabSupplier Supplies the current tab, null if the search UI will be shown in a
     *     separate activity.
     * @param url The URL used to address the SearchPage.
     */
    public WootzAppSearchPage(
            Activity activity,
            NativePageHost host,
            SnackbarManager snackbarManager,
            Profile profile,
            BottomSheetController bottomSheetController,
            Supplier<Tab> tabSupplier,
            String url) {
        super(host);

        // Create SearchManager - this is the main controller for search functionality
        mWootzAppSearchManager =
                new WootzAppSearchManager(
                        activity,
                        /* isSeparateActivity= */ false,
                        snackbarManager,
                        profile,
                        () -> bottomSheetController,
                        tabSupplier);
        
        // Set the title for the page
        mTitle = host.getContext().getResources().getString(R.string.search_title);

        // Initialize the view - this is required by BasicNativePage
        initWithView(mWootzAppSearchManager.getView());
    }

    @Override
    public String getTitle() {
        return mTitle;
    }

    @Override
    public String getHost() {
        // This tells the system this page handles "search" host URLs
        return UrlConstants.WOOTZAPP_SEARCH_HOST;
    }

    @Override
    public void destroy() {
        // Clean up resources when page is destroyed
        mWootzAppSearchManager.onDestroyed();
        mWootzAppSearchManager = null;
        super.destroy();
    }

    // For testing purposes
    public WootzAppSearchManager getWootzAppSearchManagerForTesting() {
        return mWootzAppSearchManager;
    }
}