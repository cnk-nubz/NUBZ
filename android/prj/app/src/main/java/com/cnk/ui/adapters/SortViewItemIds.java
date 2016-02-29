package com.cnk.ui.adapters;

import com.cnk.R;

public class SortViewItemIds {

    private Integer textViewId;
    private Integer upButtonId;
    private Integer downButtonId;

    public SortViewItemIds() {
        textViewId = R.id.text;
        upButtonId = R.id.upButton;
        downButtonId = R.id.downButton;
    }

    public SortViewItemIds(Integer textViewId, Integer upButtonId, Integer downButtonId) {
        this.textViewId = textViewId;
        this.upButtonId = upButtonId;
        this.downButtonId = downButtonId;
    }

    public Integer getTextViewId() {
        return textViewId;
    }

    public void setTextViewId(Integer textViewId) {
        this.textViewId = textViewId;
    }

    public Integer getUpButtonId() {
        return upButtonId;
    }

    public void setUpButtonId(Integer upButtonId) {
        this.upButtonId = upButtonId;
    }

    public Integer getDownButtonId() {
        return downButtonId;
    }

    public void setDownButtonId(Integer downButtonId) {
        this.downButtonId = downButtonId;
    }
}
