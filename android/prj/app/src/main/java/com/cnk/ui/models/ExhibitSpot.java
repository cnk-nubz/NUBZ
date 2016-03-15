package com.cnk.ui.models;

import com.cnk.ui.AutoResizeTextView;
import com.qozix.tileview.hotspots.HotSpot;

public class ExhibitSpot extends HotSpot {
    private Integer exhibitId;
    private Integer listId;
    private String name;
    private AutoResizeTextView exhibitTextView;

    public ExhibitSpot(Integer exhibitId,
                       Integer listId,
                       String name,
                       AutoResizeTextView exhibitTextView) {
        super();
        this.listId = listId;
        this.exhibitId = exhibitId;
        this.name = name;
        this.exhibitTextView = exhibitTextView;
    }

    public Integer getExhibitId() {
        return exhibitId;
    }

    public String getName() {
        return name;
    }

    public AutoResizeTextView getExhibitTextView() {
        return exhibitTextView;
    }

    public Integer getListId() {
        return listId;
    }
}