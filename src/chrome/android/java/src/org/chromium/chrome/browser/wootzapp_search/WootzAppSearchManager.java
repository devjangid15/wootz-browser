// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.wootzapp_search;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager2.widget.ViewPager2;

import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;

import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.components.browser_ui.bottomsheet.BottomSheetController;

import java.util.List;

/** Manages the search UI with separate Web and AI views. */
public class WootzAppSearchManager {
    private static final String TAG = "WootzAppSearchManager";
    
    private final FragmentActivity mActivity;
    private final boolean mIsSeparateActivity;
    private final SnackbarManager mSnackbarManager;
    private final Profile mProfile;
    private final Supplier<Tab> mTabSupplier;

    private ViewGroup mRootView;
    private EditText mSearchInput;
    private TabLayout mSearchTabs;
    private ViewPager2 mContentPager;
    private SearchPagerAdapter mPagerAdapter;

    /**
     * Creates a new WootzAppSearchManager.
     */
    public WootzAppSearchManager(
            @NonNull Activity activity,
            boolean isSeparateActivity,
            @NonNull SnackbarManager snackbarManager,
            @NonNull Profile profile,
            @Nullable Supplier<BottomSheetController> bottomSheetController,
            @Nullable Supplier<Tab> tabSupplier) {
        
        Log.d(TAG, "Constructor called - Activity: " + activity);
        
        mActivity = (FragmentActivity) activity;
        mIsSeparateActivity = isSeparateActivity;
        mSnackbarManager = snackbarManager;
        mProfile = profile;
        mTabSupplier = tabSupplier;

        // Create the main view using FrameLayout
        mRootView = new FrameLayout(mActivity);
        Log.d(TAG, "Root view created: " + mRootView);
        
        // Inflate the search layout
        View searchView = LayoutInflater.from(activity).inflate(R.layout.search_main, null);
        mRootView.addView(searchView);
        Log.d(TAG, "Search layout inflated and added to root view");

        // Initialize UI components
        initializeViews(searchView);
        setupSearchInput();
        setupViewPager();
        
        Log.d(TAG, "WootzAppSearchManager initialization completed");
    }

    /** Initialize all view references. */
    private void initializeViews(View searchView) {
        Log.d(TAG, "Initializing views from searchView: " + searchView);
        
        mSearchInput = searchView.findViewById(R.id.search_input);
        mSearchTabs = searchView.findViewById(R.id.search_tabs);
        mContentPager = searchView.findViewById(R.id.content_pager);
        
        Log.d(TAG, "Views initialized - SearchInput: " + mSearchInput + 
              ", SearchTabs: " + mSearchTabs + ", ContentPager: " + mContentPager);
    }

    /** Setup ViewPager2 with fragments and tab change listener. */
    private void setupViewPager() {
        Log.d(TAG, "Setting up ViewPager2");
        
        mPagerAdapter = new SearchPagerAdapter(mActivity);
        mContentPager.setAdapter(mPagerAdapter);
        Log.d(TAG, "PagerAdapter created: " + mPagerAdapter + ", Activity: " + mActivity);

        // Connect TabLayout with ViewPager2
        new TabLayoutMediator(mSearchTabs, mContentPager, (tab, position) -> {
            String tabText = position == 0 ? "Web" : "AI";
            tab.setText(tabText);
            Log.d(TAG, "Tab created at position " + position + " with text: " + tabText);
        }).attach();

        // Add page change listener to handle AI tab switching
        mContentPager.registerOnPageChangeCallback(new ViewPager2.OnPageChangeCallback() {
            @Override
            public void onPageSelected(int position) {
                Log.d(TAG, "Page selected: " + position);
                if (position == 1 && !mCurrentSearchQuery.isEmpty()) {
                    // AI tab selected and we have a search query
                    Log.d(TAG, "AI tab selected with query: " + mCurrentSearchQuery);
                    mContentPager.post(() -> {
                        AiChatFragment aiFragment = getAiFragment();
                        if (aiFragment != null) {
                            Log.d(TAG, "Auto-searching in AI tab with query: " + mCurrentSearchQuery);
                            aiFragment.sendMessage(mCurrentSearchQuery);
                        } else {
                            Log.e(TAG, "AiFragment is null when switching to AI tab");
                        }
                    });
                }
            }
        });

        Log.d(TAG, "ViewPager2 setup completed");
    }

    /** Setup search input functionality. */
    private void setupSearchInput() {
        Log.d(TAG, "Setting up search input: " + mSearchInput);
        
        if (mSearchInput == null) {
            Log.e(TAG, "Search input is null!");
            return;
        }
        
        mSearchInput.setOnEditorActionListener((v, actionId, event) -> {
            Log.d(TAG, "Editor action triggered - actionId: " + actionId + 
                  ", EditorInfo.IME_ACTION_SEARCH: " + EditorInfo.IME_ACTION_SEARCH);
            
            if (actionId == EditorInfo.IME_ACTION_SEARCH) {
                String query = mSearchInput.getText().toString().trim();
                Log.d(TAG, "Search action detected with query: '" + query + "'");
                
                if (!TextUtils.isEmpty(query)) {
                    Log.d(TAG, "Query is not empty, performing search");
                    performSearch(query);
                } else {
                    Log.d(TAG, "Query is empty, ignoring search action");
                }
                return true;
            }
            Log.d(TAG, "Not a search action, returning false");
            return false;
        });
        // Add this after the existing setupSearchInput code
        
        mSearchInput.setOnClickListener(v -> {
            Log.d(TAG, "Search input clicked!");
        });

        mSearchInput.setOnFocusChangeListener((v, hasFocus) -> {
            Log.d(TAG, "Search input focus changed - hasFocus: " + hasFocus);
        });

        Log.d(TAG, "Search input setup completed");
    }

    /** Perform search in both Web and AI modes. */
    private void performSearch(String query) {
        Log.d(TAG, "performSearch called with query: '" + query + "'");
        
        // Close keyboard
        InputMethodManager imm = (InputMethodManager) mActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm != null) {
            imm.hideSoftInputFromWindow(mSearchInput.getWindowToken(), 0);
            Log.d(TAG, "Keyboard closed after search");
        }

        // Store the current search query
        mCurrentSearchQuery = query;
        Log.d(TAG, "Stored current search query: " + mCurrentSearchQuery);

        // Switch to Web tab to show results
        mContentPager.setCurrentItem(0, true);
        Log.d(TAG, "Switched to Web tab (position 0)");
        
        // Use fragment tags to get existing fragments
        mContentPager.post(() -> {
            Log.d(TAG, "Posting search to fragments using tags");
            
            // Get fragments by tag
            WebSearchFragment webFragment = getWebFragment();
            AiChatFragment aiFragment = getAiFragment();
            
            Log.d(TAG, "Retrieved fragments by tag - WebFragment: " + webFragment + ", AiFragment: " + aiFragment);
            
            if (webFragment != null) {
                Log.d(TAG, "Calling loadSearchResults on WebFragment");
                webFragment.loadSearchResults(query);
            } else {
                Log.e(TAG, "WebFragment is null! Cannot load search results");
            }
            
            if (aiFragment != null) {
                Log.d(TAG, "Calling sendMessage on AiFragment");
                aiFragment.sendMessage(query);
            } else {
                Log.e(TAG, "AiFragment is null! Cannot send message to AI");
            }
        });
        
        Log.d(TAG, "performSearch completed");
    }

    // Add this field to store current search query
    private String mCurrentSearchQuery = "";

    /** Get the current search query. */
    public String getCurrentSearchQuery() {
        return mCurrentSearchQuery;
    }

    /** Get fragment by class type. */
    private Fragment getFragmentByClass(Class<?> fragmentClass) {
        Log.d(TAG, "Getting fragment by class: " + fragmentClass.getSimpleName());
        
        List<Fragment> fragments = mActivity.getSupportFragmentManager().getFragments();
        Log.d(TAG, "Total fragments in manager: " + fragments.size());
        
        for (Fragment fragment : fragments) {
            Log.d(TAG, "Checking fragment: " + fragment + ", class: " + fragment.getClass().getSimpleName());
            
            if (fragmentClass.isInstance(fragment)) {
                Log.d(TAG, "Found fragment of type " + fragmentClass.getSimpleName() + ": " + fragment);
                return fragment;
            }
        }
        
        Log.d(TAG, "No fragment found of type " + fragmentClass.getSimpleName());
        return null;
    }

    /** Get Web fragment by class. */
    private WebSearchFragment getWebFragment() {
        Fragment fragment = getFragmentByClass(WebSearchFragment.class);
        if (fragment instanceof WebSearchFragment) {
            Log.d(TAG, "Successfully retrieved WebSearchFragment");
            return (WebSearchFragment) fragment;
        } else {
            Log.e(TAG, "Fragment is not WebSearchFragment: " + fragment);
            return null;
        }
    }

    /** Get AI fragment by class. */
    private AiChatFragment getAiFragment() {
        Fragment fragment = getFragmentByClass(AiChatFragment.class);
        if (fragment instanceof AiChatFragment) {
            Log.d(TAG, "Successfully retrieved AiChatFragment");
            return (AiChatFragment) fragment;
        } else {
            Log.e(TAG, "Fragment is not AiChatFragment: " + fragment);
            return null;
        }
    }

    /** @return The view that shows the main search UI. */
    public ViewGroup getView() {
        Log.d(TAG, "getView called, returning: " + mRootView);
        return mRootView;
    }

    /** Called when the activity/native page is destroyed. */
    public void onDestroyed() {
        Log.d(TAG, "onDestroyed called");
        // Clean up resources if needed
    }
}