// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.wootzapp_search;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebSettings;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import org.chromium.chrome.R;

/** Fragment for displaying web search results. */
public class WebSearchFragment extends Fragment {
    private static final String TAG = "WebSearchFragment";
    
    private WebView mWebView;
    private String mSearchQuery;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater,
                             @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        Log.d(TAG, "onCreateView called");
        
        View root = inflater.inflate(R.layout.fragment_web_search, container, false);
        Log.d(TAG, "Fragment layout inflated: " + root);
        
        mWebView = root.findViewById(R.id.web_search_results);
        Log.d(TAG, "WebView found: " + mWebView);
        
        setupWebView();
        
        return root;
    }

    private void setupWebView() {
        Log.d(TAG, "Setting up WebView: " + mWebView);
        
        if (mWebView == null) {
            Log.e(TAG, "WebView is null! Cannot setup");
            return;
        }
        
        // Configure WebView settings
        WebSettings settings = mWebView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setLoadWithOverviewMode(true);
        settings.setUseWideViewPort(true);
        settings.setBuiltInZoomControls(true);
        settings.setDisplayZoomControls(false);
        settings.setSupportZoom(true);
        settings.setDefaultTextEncodingName("utf-8");
        Log.d(TAG, "WebView settings configured");

        // Set WebViewClient to handle navigation
        mWebView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                Log.d(TAG, "shouldOverrideUrlLoading called with URL: " + url);
                view.loadUrl(url);
                return true;
            }

            @Override
            public void onPageStarted(WebView view, String url, android.graphics.Bitmap favicon) {
                Log.d(TAG, "Page started loading: " + url);
            }

            @Override
            public void onPageFinished(WebView view, String url) {
                Log.d(TAG, "Page finished loading: " + url);
            }

            @Override
            public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
                Log.e(TAG, "WebView error - Code: " + errorCode + ", Description: " + description + ", URL: " + failingUrl);
            }
        });

        // Load initial page
        if (mSearchQuery != null && !mSearchQuery.isEmpty()) {
            Log.d(TAG, "Loading search results for existing query: " + mSearchQuery);
            loadSearchResults(mSearchQuery);
        } else {
            Log.d(TAG, "Loading default Google page");
            mWebView.loadUrl("https://www.google.com");
        }
        
        Log.d(TAG, "WebView setup completed");
    }

    /** Load search results for the given query. */
    public void loadSearchResults(String query) {
        Log.d(TAG, "loadSearchResults called with query: '" + query + "'");
        
        if (mWebView == null) {
            Log.e(TAG, "WebView is null! Cannot load search results");
            return;
        }
        
        if (query == null || query.trim().isEmpty()) {
            Log.e(TAG, "Query is null or empty! Cannot load search results");
            return;
        }
        
        mSearchQuery = query;
        String searchUrl = "https://www.google.com/search?q=" + 
                          java.net.URLEncoder.encode(query, java.nio.charset.StandardCharsets.UTF_8);
        
        Log.d(TAG, "Loading URL: " + searchUrl);
        mWebView.loadUrl(searchUrl);
        Log.d(TAG, "loadUrl called on WebView");
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d(TAG, "onPause called");
        if (mWebView != null) {
            mWebView.onPause();
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG, "onResume called");
        if (mWebView != null) {
            mWebView.onResume();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy called");
        if (mWebView != null) {
            mWebView.destroy();
        }
    }
} 