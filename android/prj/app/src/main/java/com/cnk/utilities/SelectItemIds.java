package com.cnk.utilities;

import com.cnk.R;

public class SelectItemIds {
    private Integer textView;

    public SelectItemIds() {
        textView = R.id.text;
    }

    public SelectItemIds(Integer textView) {
        this.textView = textView;
    }

    public Integer getTextView() {
        return textView;
    }

    public void setTextView(Integer textView) {
        this.textView = textView;
    }
}
