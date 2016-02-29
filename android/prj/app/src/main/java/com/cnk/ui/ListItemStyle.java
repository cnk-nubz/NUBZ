package com.cnk.ui;

import android.graphics.Color;
import android.view.Gravity;
import android.widget.TextView;

public class ListItemStyle {

    private int gravity;
    private int maxLines;
    private int height;
    private int activeColor;
    private int notActiveColor;
    private float textSize;

    public void apply(TextView view, Boolean selected) {
        view.setGravity(gravity);
        view.setMaxLines(maxLines);
        view.setHeight(height);
        view.setTextSize(textSize);
        view.setBackgroundColor(selected ? activeColor : notActiveColor);
    }

    public ListItemStyle() {
        gravity = Gravity.CENTER;
        maxLines = 3;
        height = 80;
        textSize = 20f;
        activeColor = Color.GREEN;
        notActiveColor = 0xd3d3d3d3;
    }

    public void setGravity(int gravity) {
        this.gravity = gravity;
    }

    public void setMaxLines(int maxLines) {
        this.maxLines = maxLines;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public void setTextSize(float textSize) {
        this.textSize = textSize;
    }

    public void setActiveColor(int activeColor) {
        this.activeColor = activeColor;
    }

    public void setNotActiveColor(int notActiveColor) {
        this.notActiveColor = notActiveColor;
    }

    public int getNotActiveColor() {
        return notActiveColor;
    }

    public int getActiveColor() {
        return activeColor;
    }
}
