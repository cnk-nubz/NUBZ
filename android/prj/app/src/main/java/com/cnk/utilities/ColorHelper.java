package com.cnk.utilities;

import android.graphics.Color;

public class ColorHelper {

    public static int getArgbHexFromRGB(int colorR, int colorG, int colorB) {
        return Color.argb(0xff, colorR, colorG, colorB);
    }

    public static int getArgbBorderHexFromRGH(int colorR, int colorG, int colorB) {
        return Color.argb(0xff, Math.max(0, colorR - 30), Math.max(0, colorG - 30), Math.max(0, colorB - 30));
    }

    public static int getRgbHexTextColorForBackground(int colorR, int colorG, int colorB) {
        if (colorR + colorG + colorB < 383) {
            return Color.WHITE;
        } else {
            return Color.BLACK;
        }
    }
}
