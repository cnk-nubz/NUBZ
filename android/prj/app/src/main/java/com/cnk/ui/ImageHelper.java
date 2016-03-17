package com.cnk.ui;

public class ImageHelper {
    public static Integer getDimensionWhenScaleApplied(Integer dimensionToScale,
                                                       Integer exampleOriginalValue,
                                                       Integer exampleScaled) {
        Float scale = (float) exampleScaled / exampleOriginalValue;
        return (int) (dimensionToScale * scale);
    }
}
