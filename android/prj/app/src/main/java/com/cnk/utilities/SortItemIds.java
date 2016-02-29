package com.cnk.utilities;

import com.cnk.R;

public class SortItemIds {

    private Integer textView;
    private Integer upButton;
    private Integer downButton;

    public SortItemIds() {
        textView = R.id.text;
        upButton = R.id.upButton;
        downButton = R.id.downButton;
    }

    public SortItemIds(Integer textView, Integer upButton, Integer downButton) {
        this.textView = textView;
        this.upButton = upButton;
        this.downButton = downButton;
    }

    public Integer getTextView() {
        return textView;
    }

    public void setTextView(Integer textView) {
        this.textView = textView;
    }

    public Integer getUpButton() {
        return upButton;
    }

    public void setUpButton(Integer upButton) {
        this.upButton = upButton;
    }

    public Integer getDownButton() {
        return downButton;
    }

    public void setDownButton(Integer downButton) {
        this.downButton = downButton;
    }
}
