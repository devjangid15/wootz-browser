// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.wootzapp_search;

import android.util.Log;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;

/** Adapter for ViewPager2 to manage Web and AI fragments. */
public class SearchPagerAdapter extends FragmentStateAdapter {
    private static final String TAG = "SearchPagerAdapter";
    private static final int TAB_COUNT = 2;

    public SearchPagerAdapter(@NonNull FragmentActivity fragmentActivity) {
        super(fragmentActivity);
        Log.d(TAG, "SearchPagerAdapter created with activity: " + fragmentActivity);
    }

    @NonNull
    @Override
    public Fragment createFragment(int position) {
        Log.d(TAG, "createFragment called for position: " + position);
        
        Fragment fragment;
        switch (position) {
            case 0:
                fragment = new WebSearchFragment();
                Log.d(TAG, "Created WebSearchFragment: " + fragment);
                break;
            case 1:
                fragment = new AiChatFragment();
                Log.d(TAG, "Created AiChatFragment: " + fragment);
                break;
            default:
                Log.e(TAG, "Invalid position: " + position);
                throw new IllegalArgumentException("Invalid position: " + position);
        }
        
        return fragment;
    }

    @Override
    public int getItemCount() {
        Log.d(TAG, "getItemCount called, returning: " + TAB_COUNT);
        return TAB_COUNT;
    }

    @Override
    public long getItemId(int position) {
        Log.d(TAG, "getItemId called for position: " + position);
        return position;
    }

    @Override
    public boolean containsItem(long itemId) {
        Log.d(TAG, "containsItem called for itemId: " + itemId);
        return itemId >= 0 && itemId < TAB_COUNT;
    }
} 