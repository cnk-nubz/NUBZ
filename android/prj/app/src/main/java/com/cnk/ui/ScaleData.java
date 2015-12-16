package com.cnk.ui;

public class ScaleData {
    private Float scaleValue;
    private Integer scaleCode;

    public ScaleData(Float scaleValue, Integer scaleCode) {
        this.scaleValue = scaleValue;
        this.scaleCode = scaleCode;
    }

    public Float getScaleValue() {
        return scaleValue;
    }

    public Integer getScaleCode() {
        return scaleCode;
    }
}
