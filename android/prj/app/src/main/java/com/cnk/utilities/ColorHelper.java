package com.cnk.utilities;

import android.graphics.Color;

public class ColorHelper {

    public static int getRgbBorderHexForColor(int color) {
        return Color.rgb(Math.max(0, Color.red(color) - 30),
                         Math.max(0, Color.green(color) - 30),
                         Math.max(0, Color.blue(color) - 30));
    }

    public static int getRgbHexTextColorForBackground(int background) {
        if (Color.red(background) + Color.green(background) + Color.blue(background) < 383) {
            return Color.WHITE;
        } else {
            return Color.BLACK;
        }
    }
}
