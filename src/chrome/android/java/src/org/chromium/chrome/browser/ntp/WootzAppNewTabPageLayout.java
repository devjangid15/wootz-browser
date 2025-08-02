// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.ntp;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import org.chromium.chrome.R;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.chromium.chrome.browser.extensions.Extensions;
import org.chromium.chrome.browser.extensions.ExtensionInfo;
import org.chromium.chrome.browser.extensions.OpenExtensionsById;

import java.util.List;

/**
 * Custom NTP layout that shows a colored background and extension tiles.
 */
public class WootzAppNewTabPageLayout extends NewTabPageLayout {
    private static final String TAG = "WootzAppNewTabPageLayout";

    // Grid for extension tiles
    private GridLayout mExtensionsGrid;

    public WootzAppNewTabPageLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.e(TAG, "WootzAppNewTabPageLayout constructor");
    }

    // @Override
    // protected void onFinishInflate() {
    //     super.onFinishInflate();

    //     setBackgroundColor(Color.parseColor("#FFA500")); // Orange
    //     setupExtensionTiles();
    //     // mMiddleSpacer = findViewById(R.id.ntp_middle_spacer);
    //     // mFakeSearchBoxLayout = findViewById(R.id.search_box);
    // }

    private void setupExtensionTiles() {
        // Find the container for MV tiles (site section)
        ViewGroup mvTilesContainerLayout = getMvTilesContainerLayout();
        if (mvTilesContainerLayout == null) {
            Log.e(TAG, "mvTilesContainerLayout is null!");
            return;
        }

        // Remove any previous views
        // mvTilesContainerLayout.removeAllViews();

        // Create the grid
        mExtensionsGrid = new GridLayout(getContext());
        mExtensionsGrid.setColumnCount(3); // 3 columns, adjust as needed
        mExtensionsGrid.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        mExtensionsGrid.setPadding(16, 16, 16, 16);

        List<ExtensionInfo> extensions = Extensions.getExtensionsInfo();
        if (extensions == null || extensions.isEmpty()) {
            TextView empty = new TextView(getContext());
            empty.setText("No extensions installed");
            empty.setTextColor(Color.WHITE);
            empty.setTextSize(16);
            mExtensionsGrid.addView(empty);
                        } else {
            for (ExtensionInfo extension : extensions) {
                View tile = createExtensionTile(extension);
                mExtensionsGrid.addView(tile);
            }
        }

        mvTilesContainerLayout.addView(mExtensionsGrid);
    }

    private View createExtensionTile(ExtensionInfo extension) {
        Context context = getContext();
        LinearLayout tile = new LinearLayout(context);
        tile.setOrientation(LinearLayout.VERTICAL);
        tile.setPadding(16, 16, 16, 16);
        tile.setGravity(Gravity.CENTER);

        ImageView icon = new ImageView(context);
        icon.setImageResource(R.drawable.btn_back);

        int iconSize = (int) (48 * getResources().getDisplayMetrics().density);
        LinearLayout.LayoutParams iconParams = new LinearLayout.LayoutParams(iconSize, iconSize);
        icon.setLayoutParams(iconParams);

        TextView name = new TextView(context);
        name.setText(extension.getName());
        name.setTextColor(Color.WHITE);
        name.setTextSize(14);
        name.setGravity(Gravity.CENTER);

        tile.addView(icon);
        tile.addView(name);

        tile.setOnClickListener(v -> {
            try {
                OpenExtensionsById.openExtensionById(extension.getId());
                } catch (Exception e) {
                Log.e(TAG, "Failed to open extension: " + extension.getId(), e);
            }
        });

        GridLayout.LayoutParams params = new GridLayout.LayoutParams();
        params.width = 0;
        params.height = ViewGroup.LayoutParams.WRAP_CONTENT;
        params.columnSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f);
        params.setMargins(8, 8, 8, 8);
        tile.setLayoutParams(params);

        return tile;
    }

}
