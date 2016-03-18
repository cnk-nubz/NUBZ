package com.cnk.ui.adapters;

import com.cnk.R;

public class SelectViewItemIds {
    private Integer textViewId;

    public SelectViewItemIds() {
        textViewId = R.id.text;
    }

    public SelectViewItemIds(Integer textViewId) {
        this.textViewId = textViewId;
    }

    public Integer getTextViewId() {
        return textViewId;
    }

    public void setTextViewId(Integer textViewId) {
        this.textViewId = textViewId;
    }
}
