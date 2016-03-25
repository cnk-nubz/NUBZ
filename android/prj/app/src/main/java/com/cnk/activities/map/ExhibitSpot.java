package com.cnk.activities.map;

import android.support.annotation.NonNull;

import com.cnk.ui.AutoResizeTextView;
import com.qozix.tileview.hotspots.HotSpot;

public class ExhibitSpot extends HotSpot {
    private int exhibitId;
    private AutoResizeTextView textView;

    public ExhibitSpot(int exhibitId, @NonNull AutoResizeTextView textView) {
        super();
        this.exhibitId = exhibitId;
        this.textView = textView;
    }

    public int getExhibitId() {
        return exhibitId;
    }

    public AutoResizeTextView getTextView() {
        return textView;
    }
}