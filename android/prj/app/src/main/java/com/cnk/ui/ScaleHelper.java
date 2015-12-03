package com.cnk.ui;

import android.util.Pair;

public class ScaleHelper {
    public static final Integer maximumPixCount = 10 * 1000 * 1000;

    public static Pair<Integer, Integer> getInterpolatedDimensions(Integer width, Integer height) {
        Integer midWidth = 0, midHeight = 0;
        Integer lowerWidth = width / 10, higherWidth = 10 * width;
        Integer lowerHeight = height / 10, higherHeight = 10 * height;

        while (midWidth != (lowerWidth + higherWidth) / 2) {
            midWidth = (lowerWidth + higherWidth) / 2;
            midHeight = (lowerHeight + higherHeight) / 2;

            if (midWidth * midHeight <= maximumPixCount) {
                lowerWidth = midWidth;
                lowerHeight = midHeight;
            } else {
                higherWidth = midWidth;
                higherHeight = midHeight;
            }
        }

        return new Pair<>(lowerWidth, lowerHeight);
    }

    public static Integer getDimensionWhenScaleApplied(Integer dimensionToScale, Integer exampleOriginalValue, Integer exampleScaled) {
        Float scale = (float) exampleScaled / exampleOriginalValue;
        return (int) (dimensionToScale * scale);
    }
}
