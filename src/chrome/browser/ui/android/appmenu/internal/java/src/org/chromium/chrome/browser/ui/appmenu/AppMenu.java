// Copyright 2011 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.ui.appmenu;

import static org.chromium.build.NullUtil.assumeNonNull;

import android.animation.Animator;
import android.animation.AnimatorSet;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.SystemClock;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;
import android.view.View.MeasureSpec;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.content.DialogInterface;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.LinearLayout;

import org.chromium.ui.modelutil.MVCListAdapter;
import org.chromium.ui.modelutil.PropertyModel;
import org.chromium.ui.modelutil.MVCListAdapter.ModelList;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.bottomsheet.BottomSheetBehavior;

import androidx.annotation.VisibleForTesting;
import androidx.appcompat.content.res.AppCompatResources;

import org.chromium.base.Callback;
import org.chromium.base.SysUtils;
import org.chromium.base.metrics.RecordHistogram;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.build.annotations.EnsuresNonNullIf;
import org.chromium.build.annotations.Initializer;
import org.chromium.build.annotations.MonotonicNonNull;
import org.chromium.build.annotations.NullMarked;
import org.chromium.build.annotations.Nullable;
import org.chromium.build.annotations.RequiresNonNull;
import org.chromium.chrome.browser.browser_controls.BrowserControlsStateProvider.ControlsPosition;
import org.chromium.chrome.browser.ui.appmenu.internal.R;
import org.chromium.components.browser_ui.styles.SemanticColorUtils;
import org.chromium.components.browser_ui.widget.chips.ChipView;
import org.chromium.components.browser_ui.widget.highlight.ViewHighlighter;
import org.chromium.components.browser_ui.widget.highlight.ViewHighlighter.HighlightParams;
import org.chromium.components.browser_ui.widget.highlight.ViewHighlighter.HighlightShape;

import java.util.List;

/**
 * Shows a popup of menu items anchored to a host view.
 *
 * <p>When an item is selected, we call {@link AppMenuHandlerImpl#onOptionsItemSelected}, which then
 * delegates to {@link AppMenuDelegate#onOptionsItemSelected}.
 *
 * <ul>
 *   <li>Only visible menu items are shown.
 *   <li>Disabled items are grayed out.
 * </ul>
 */

/**
 * By Dev Jangid
 * Shows a bottom sheet dialog with menu items in a grid layout.
 * WootzApp Customized the AppMenu to show the menu items in a grid layout 
 * instead of a list in bottom sheet dialog.
 */

@NullMarked
class AppMenu implements OnKeyListener {

    /** Delegate to be notified of various visibility events from the app menu. */
    interface AppMenuVisibilityDelegate {
        /** Called when the AppMenu is dismissed. */
        void appMenuDismissed();

        /**
         * Called by AppMenu to report that the App Menu visibility has changed.
         *
         * @param isVisible Whether the App Menu is showing.
         */
        void onMenuVisibilityChanged(boolean isVisible);
    }

    /** Provides initial sizing information for the app menu. */
    interface InitialSizingHelper {
        /**
         * Get the preferred initial height for a given view.
         *
         * @param index The index of the view in the Adapter.
         * @return The recommended initial height for the view at a given index (in pixels).
         */
        int getInitialHeightForView(int index);

        /** Return whether the view at the given index can be the last initial view displayed. */
        boolean canBeLastVisibleInitialView(int index);
    }

    private static final float LAST_ITEM_SHOW_FRACTION = 0.5f;

    /** A means of reporting an exception/stack without crashing. */
    private static @MonotonicNonNull Callback<Throwable> sExceptionReporter;

    private final int mVerticalFadeDistance;
    private final int mNegativeSoftwareVerticalOffset;
    private final int mChipHighlightExtension;
    private final int[] mTempLocation;
    private final AppMenuVisibilityDelegate mVisibilityDelegate;

    private GridView mGridView;
    private static final int GRID_COLUMNS = 3;
    private GridAdapter mGridAdapter;

    private BottomSheetBehavior<View> mBehavior;
    private BottomSheetDialog mBottomSheetDialog;
    private @Nullable ListAdapter mAdapter;
    private @Nullable View mFooterView;
    private int mCurrentScreenRotation = -1;
    private boolean mIsByPermanentButton;
    private @Nullable AnimatorSet mMenuItemEnterAnimator;
    private long mMenuShownTimeMs;
    private boolean mSelectedItemBeforeDismiss;
    private InitialSizingHelper mInitialSizingHelper;

    /**
     * Creates and sets up the App Menu.
     *
     * @param visibilityDelegate The visibility delegate for the Menu.
     * @param res Resources object used to get dimensions and style attributes.
     */
    AppMenu(AppMenuVisibilityDelegate visibilityDelegate, Resources res) {
        mVisibilityDelegate = visibilityDelegate;

        mNegativeSoftwareVerticalOffset =
                res.getDimensionPixelSize(R.dimen.menu_negative_software_vertical_offset);
        mVerticalFadeDistance = res.getDimensionPixelSize(R.dimen.menu_vertical_fade_distance);
        mChipHighlightExtension =
                res.getDimensionPixelOffset(R.dimen.menu_chip_highlight_extension);

        mTempLocation = new int[2];
    }

    /**
     * Creates and shows the app menu anchored to the specified view.
     *
     * @param context The context of the AppMenu (ensure the proper theme is set on this context).
     * @param anchorView The anchor {@link View} of the {@link PopupWindow}.
     * @param isByPermanentButton Whether or not permanent hardware button triggered it. (oppose to
     *     software button or keyboard).
     * @param screenRotation Current device screen rotation.
     * @param visibleDisplayFrame The display area rect in which AppMenu is supposed to fit in.
     * @param footer The view to add as a fixed view at the bottom of the menu. Can be null if no
     *     such view is required. The footer is always visible and overlays other app menu items if
     *     necessary.
     * @param header The resource id for a view to add as the first item in menu list. Can be null
     *     if no such view is required. See {@link ListView#addHeaderView(View)}.
     * @param highlightedItemId The resource id of the menu item that should be highlighted. Can be
     *     {@code null} if no item should be highlighted. Note that {@code 0} is dedicated to custom
     *     menu items and can be declared by external apps.
     * @param isMenuIconAtStart Whether the menu is being shown from a menu icon positioned at the
     *     start.
     * @param addTopPaddingBeforeFirstRow Whether top padding is needed above the first row.
     */
    void show(
            Context context,
            final View anchorView,
            boolean isByPermanentButton,
            int screenRotation,
            Rect visibleDisplayFrame,
            @Nullable View footer,
            @Nullable View header,
            @Nullable Integer highlightedItemId,
            boolean isMenuIconAtStart,
            @ControlsPosition int controlsPosition,
            boolean addTopPaddingBeforeFirstRow,
            @Nullable ModelList modelList) {

        mBottomSheetDialog = new BottomSheetDialog(context);
        mBottomSheetDialog.setCancelable(true);
        mBottomSheetDialog.setCanceledOnTouchOutside(true);

        mBottomSheetDialog.setOnDismissListener(
                new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialog) {
                    recordTimeToTakeActionHistogram();
                    if (anchorView instanceof ImageButton) {
                        ((ImageButton) anchorView).setSelected(false);
                    }
                    if (mMenuItemEnterAnimator != null) mMenuItemEnterAnimator.cancel();
                    mVisibilityDelegate.appMenuDismissed();
                    mVisibilityDelegate.onMenuVisibilityChanged(false);
                    mMenuItemEnterAnimator = null;
                    }
                });

        // Inflate the grid layout
        LayoutInflater inflater = LayoutInflater.from(context);
        View contentView = inflater.inflate(R.layout.app_menu_grid_layout, null);
        mGridView = contentView.findViewById(R.id.app_menu_grid);
        mGridView.setNumColumns(GRID_COLUMNS);
        LinearLayout leftColumn = contentView.findViewById(R.id.app_menu_left_column);

        java.util.List<PropertyModel> rowMenuItems = new java.util.ArrayList<>();
        java.util.List<PropertyModel> gridMenuItems = new java.util.ArrayList<>();

        // Adding items to the grid and row menu items
        if (modelList != null) {
            for (int i = 0; i < modelList.size(); i++) {
                MVCListAdapter.ListItem item = modelList.get(i);
                if (item.type == AppMenuHandler.AppMenuItemType.BUTTON_ROW) {
                    ModelList subList = item.model.get(AppMenuItemProperties.ADDITIONAL_ICONS);
                    if (subList != null) {
                        for (int j = 0; j < subList.size(); j++) {
                            PropertyModel subModel = subList.get(j).model;
                            rowMenuItems.add(subModel);
                        }
                    }
                } else if ((item.type == AppMenuHandler.AppMenuItemType.STANDARD ||
                           item.type == AppMenuHandler.AppMenuItemType.TITLE_BUTTON)) {
                    gridMenuItems.add(item.model);
                }
            }
        }

        // Add row items to left column
        for (PropertyModel model : rowMenuItems) {
            View rowItemView = inflater.inflate(R.layout.app_menu_row_item, leftColumn, false);
            ImageView icon = rowItemView.findViewById(R.id.row_item_icon);

            Drawable iconDrawable = model.get(AppMenuItemProperties.ICON);
            if (iconDrawable != null) {
                icon.setImageDrawable(iconDrawable);
                icon.setVisibility(View.VISIBLE);
            } else {
                icon.setVisibility(View.GONE);
            }

            boolean isEnabled = model.get(AppMenuItemProperties.ENABLED);
            rowItemView.setEnabled(isEnabled);
            rowItemView.setAlpha(isEnabled ? 1.0f : 0.5f);

            // Set click listener with null check
            AppMenuClickHandler clickHandler = model.get(AppMenuItemProperties.CLICK_HANDLER);
                rowItemView.setOnClickListener(v -> {
                    if (!isEnabled) return;
                    clickHandler.onItemClick(model, null);
                    mBottomSheetDialog.dismiss();
                });

            leftColumn.addView(rowItemView);
        }

        mGridAdapter = new GridAdapter(context, gridMenuItems);
        mGridView.setAdapter(mGridAdapter);

        mGridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                PropertyModel model = mGridAdapter.getItem(position) instanceof PropertyModel
                        ? (PropertyModel) mGridAdapter.getItem(position)
                        : null;
                if (model != null) {
                    // Check if item is enabled
                    if (!model.get(AppMenuItemProperties.ENABLED)) return;
                    AppMenuClickHandler clickHandler = model.get(AppMenuItemProperties.CLICK_HANDLER);
                    clickHandler.onItemClick(model, null);
                }
                mBottomSheetDialog.dismiss();
            }
        });

        mBottomSheetDialog.setContentView(contentView);

        mBehavior = BottomSheetBehavior.from((View) contentView.getParent());
        mBehavior.setState(BottomSheetBehavior.STATE_EXPANDED);
        mBehavior.setDraggable(false);

        mVisibilityDelegate.onMenuVisibilityChanged(true);
        mBottomSheetDialog.show();
    }

    private class GridAdapter extends BaseAdapter {
        private final LayoutInflater mInflater;
        private final List<PropertyModel> mGridMenuItems;

        GridAdapter(Context context, List<PropertyModel> gridMenuItems) {
            mInflater = LayoutInflater.from(context);
            mGridMenuItems = gridMenuItems;
        }

        @Override
        public int getCount() {
            return mGridMenuItems.size();
        }

        @Override
        public Object getItem(int position) {
            return mGridMenuItems.get(position);
        }

        @Override
        public long getItemId(int position) {
            PropertyModel model = mGridMenuItems.get(position);
            Integer id = model.get(AppMenuItemProperties.MENU_ITEM_ID);
            return id != null ? id : position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            View view = convertView;
            if (view == null) {
                view = mInflater.inflate(R.layout.app_menu_grid_item, parent, false);
            }
            PropertyModel model = mGridMenuItems.get(position);
            ImageView icon = view.findViewById(R.id.grid_item_icon);
            TextView title = view.findViewById(R.id.grid_item_title);

            Drawable iconDrawable = model.get(AppMenuItemProperties.ICON);
            if (iconDrawable != null) {
                icon.setImageDrawable(iconDrawable);
                icon.setVisibility(View.VISIBLE);
            } else {
                icon.setVisibility(View.GONE);
            }
            title.setText(model.get(AppMenuItemProperties.TITLE));
            return view;
        }
    }

    @VisibleForTesting
    static int[] getPopupPosition(
            int[] tempLocation,
            boolean isByPermanentButton,
            int negativeSoftwareVerticalOffset,
            int screenRotation,
            Rect appRect,
            Rect padding,
            View anchorView,
            int popupWidth,
            int viewLayoutDirection) {
        anchorView.getLocationInWindow(tempLocation);
        int anchorViewX = tempLocation[0];
        int anchorViewY = tempLocation[1];

        int[] offsets = new int[2];
        // If we have a hardware menu button, locate the app menu closer to the estimated
        // hardware menu button location.
        if (isByPermanentButton) {
            int horizontalOffset = -anchorViewX;
            switch (screenRotation) {
                case Surface.ROTATION_0:
                case Surface.ROTATION_180:
                    horizontalOffset += (appRect.width() - popupWidth) / 2;
                    break;
                case Surface.ROTATION_90:
                    horizontalOffset += appRect.width() - popupWidth;
                    break;
                case Surface.ROTATION_270:
                    break;
                default:
                    assert false;
                    break;
            }
            offsets[0] = horizontalOffset;
            // The menu is displayed above the anchored view, so shift the menu up by the bottom
            // padding of the background.
            offsets[1] = -padding.bottom;
        } else {
            offsets[1] = -negativeSoftwareVerticalOffset;
            if (viewLayoutDirection != View.LAYOUT_DIRECTION_RTL) {
                offsets[0] = anchorView.getWidth() - popupWidth;
            }
        }

        int xPos = anchorViewX + offsets[0];
        int yPos = anchorViewY + offsets[1];
        int[] position = {xPos, yPos};
        return position;
    }

    /** Marks whether an item was selected prior to dismissal. */
    public void setSelectedItemBeforeDismiss(boolean selected) {
        mSelectedItemBeforeDismiss = selected;
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (mGridView == null) return false;
        if (event.getKeyCode() == KeyEvent.KEYCODE_MENU) {
            if (event.getAction() == KeyEvent.ACTION_DOWN && event.getRepeatCount() == 0) {
                event.startTracking();
                v.getKeyDispatcherState().startTracking(event, this);
                return true;
            } else if (event.getAction() == KeyEvent.ACTION_UP) {
                v.getKeyDispatcherState().handleUpEvent(event);
                if (event.isTracking() && !event.isCanceled()) {
                    dismiss();
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Update the menu items.
     *
     * @param sizingHelper The initial sizing helper for the menu.
     * @param adapter The adapter for visible items in the Menu.
     */
    @Initializer
    void updateMenu(InitialSizingHelper sizingHelper, ListAdapter adapter) {
        mInitialSizingHelper = sizingHelper;
        mAdapter = adapter;
    }

    /** Dismisses the app menu and cancels the drag-to-scroll if it is taking place. */
    void dismiss() {
        if (mBottomSheetDialog != null && mBottomSheetDialog.isShowing()) {
            mBottomSheetDialog.dismiss();
        }
    }

    /**
     * @return Whether the app menu is currently showing.
     */
    boolean isShowing() {
        return mBottomSheetDialog != null && mBottomSheetDialog.isShowing();
    }

    /**
     * @return {@link ListView} that contains all of the menu options.
     */
    @Nullable ListView getListView() {
        return null; // No ListView in BottomSheetDialog
    }

    @RequiresNonNull("mBottomSheetDialog")
    private void setMenuHeight(
            InitialSizingHelper sizingHelper,
            Rect appDimensions,
            Rect padding,
            int footerHeight,
            int headerHeight,
            View anchorView,
            int anchorViewOffset) {
        assert mAdapter != null;
        int anchorViewImpactHeight = mIsByPermanentButton ? anchorView.getHeight() : 0;

        int availableScreenSpace =
                appDimensions.height()
                        - anchorViewOffset
                        - padding.bottom
                        - footerHeight
                        - headerHeight
                        - anchorViewImpactHeight;

        if (mIsByPermanentButton) availableScreenSpace -= padding.top;
        if (availableScreenSpace <= 0 && sExceptionReporter != null) {
            String logMessage =
                    "there is no screen space for app menu, mIsByPermanentButton = "
                            + mIsByPermanentButton
                            + ", anchorViewOffset = "
                            + anchorViewOffset
                            + ", appDimensions.height() = "
                            + appDimensions.height()
                            + ", anchorView.getHeight() = "
                            + anchorView.getHeight()
                            + " padding.top = "
                            + padding.top
                            + ", padding.bottom = "
                            + padding.bottom
                            + ", footerHeight = "
                            + footerHeight
                            + ", headerHeight = "
                            + headerHeight;
            PostTask.postTask(
                    TaskTraits.BEST_EFFORT_MAY_BLOCK,
                    () -> sExceptionReporter.onResult(new Throwable(logMessage)));
        }

        // Find the height for each menu item.
        int itemCount = mAdapter == null ? 0 : mAdapter.getCount();
        int[] heightList = new int[itemCount];
        boolean[] canBeLastList = new boolean[itemCount];
        for (int i = 0; i < itemCount; i++) {
            heightList[i] = sizingHelper.getInitialHeightForView(i);
            canBeLastList[i] = sizingHelper.canBeLastVisibleInitialView(i);
        }

        int menuHeight = calculateHeightForItems(heightList, canBeLastList, availableScreenSpace);
        menuHeight += footerHeight + headerHeight + padding.top + padding.bottom;
        // mPopup.setHeight(menuHeight); // Removed mPopup
    }

    @VisibleForTesting
    static int calculateHeightForItems(
            int[] heightList, boolean[] canBeLastVisibleList, int screenSpaceForItems) {
        int availableScreenSpace = screenSpaceForItems > 0 ? screenSpaceForItems : 0;
        int spaceForFullItems = 0;

        assert heightList.length == canBeLastVisibleList.length;
        for (int height : heightList) {
            spaceForFullItems += height;
        }

        int menuHeight;
        // Fade out the last item if we cannot fit all items.
        if (availableScreenSpace < spaceForFullItems) {
            int spaceForItems = 0;
            int lastItem = 0;
            // App menu should show 1 full item at least.
            do {
                spaceForItems += heightList[lastItem++];
                if (spaceForItems + heightList[lastItem] > availableScreenSpace) {
                    break;
                }
            } while (lastItem < heightList.length - 1);

            int spaceForPartialItem = (int) (LAST_ITEM_SHOW_FRACTION * heightList[lastItem]);
            // Determine which item needs hiding. We only show Partial of the last item, if there is
            // not enough screen space to partially show the last identified item, then partially
            // show the second to last item instead. We also do not show the partial divider line.
            while (lastItem > 1
                    && (spaceForItems + spaceForPartialItem > availableScreenSpace
                            || !canBeLastVisibleList[lastItem])) {
                // If we have space for < 2.5 items, size menu to available screen space.
                if (spaceForItems <= availableScreenSpace && lastItem < 3) {
                    spaceForPartialItem = availableScreenSpace - spaceForItems;
                    break;
                }
                spaceForItems -= heightList[lastItem - 1];
                spaceForPartialItem = (int) (LAST_ITEM_SHOW_FRACTION * heightList[lastItem - 1]);
                lastItem--;
            }

            menuHeight = spaceForItems + spaceForPartialItem;
        } else {
            menuHeight = spaceForFullItems;
        }
        return menuHeight;
    }

    @RequiresNonNull("mGridView")
    private void runMenuItemEnterAnimations() {
        mMenuItemEnterAnimator = new AnimatorSet();
        AnimatorSet.Builder builder = null;

        ViewGroup list = mGridView;
        for (int i = 0; i < list.getChildCount(); i++) {
            View view = list.getChildAt(i);
            Object animatorObject = view.getTag(R.id.menu_item_enter_anim_id);
            if (animatorObject != null) {
                if (builder == null) {
                    builder = mMenuItemEnterAnimator.play((Animator) animatorObject);
                } else {
                    builder.with((Animator) animatorObject);
                }
            }
        }

        mMenuItemEnterAnimator.start();
    }

    private View createAppMenuContentView(Context context, boolean addTopPaddingBeforeFirstRow) {
        ViewGroup contentView =
                (ViewGroup) LayoutInflater.from(context).inflate(R.layout.app_menu_layout, null);
        if (addTopPaddingBeforeFirstRow) {
            contentView.setBackgroundResource(R.drawable.default_popup_menu_bg);
        } else {
            contentView.setBackgroundResource(R.drawable.app_menu_bottom_padding_bg);
        }
        return contentView;
    }

    private int attachFooter(@Nullable View footer, ViewGroup contentView, int menuWidth) {
        if (footer == null) {
            mFooterView = null;
            return 0;
        }

        mFooterView = footer;
        mFooterView.setId(R.id.app_menu_footer);
        contentView.addView(
                footer, contentView.indexOfChild(contentView.findViewById(R.id.app_menu_list)) + 1);

        int widthMeasureSpec = MeasureSpec.makeMeasureSpec(menuWidth, MeasureSpec.EXACTLY);
        int heightMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        mFooterView.measure(widthMeasureSpec, heightMeasureSpec);

        return mFooterView.getMeasuredHeight();
    }

    @RequiresNonNull("mGridView")
    private int attachHeader(@Nullable View header, int menuWidth) {
        if (header == null) return 0;

        // mListView.addHeaderView(header); // mListView is removed

        int widthMeasureSpec = MeasureSpec.makeMeasureSpec(menuWidth, MeasureSpec.EXACTLY);
        int heightMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        header.measure(widthMeasureSpec, heightMeasureSpec);

        return header.getMeasuredHeight();
    }

    void finishAnimationsForTests() {
        if (mMenuItemEnterAnimator != null) mMenuItemEnterAnimator.end();
    }

    private void recordTimeToTakeActionHistogram() {
        final String histogramName =
                "Mobile.AppMenu.TimeToTakeAction."
                        + (mSelectedItemBeforeDismiss ? "SelectedItem" : "Abandoned");
        final long timeToTakeActionMs = SystemClock.elapsedRealtime() - mMenuShownTimeMs;
        RecordHistogram.deprecatedRecordMediumTimesHistogram(histogramName, timeToTakeActionMs);
    }

    /**
     * @param reporter A means of reporting an exception without crashing.
     */
    static void setExceptionReporter(Callback<Throwable> reporter) {
        sExceptionReporter = reporter;
    }
}
