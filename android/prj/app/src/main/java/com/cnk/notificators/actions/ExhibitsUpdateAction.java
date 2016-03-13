package com.cnk.notificators.actions;

import com.cnk.database.models.Exhibit;

import java.util.List;

public interface ExhibitsUpdateAction {
    void doOnUpdate(List<Exhibit> changedExhibits);
}

