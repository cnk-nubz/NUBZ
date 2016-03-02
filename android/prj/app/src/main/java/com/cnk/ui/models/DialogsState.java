package com.cnk.ui.models;

import android.widget.Button;

import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.exhibitwindow.ExhibitActionsAdapter;

public class DialogsState {
    public AutoResizeTextView tvBreakDialogName, tvExhibitDialogName;
    public ExhibitActionsAdapter breakAdapter, exhibitAdapter;
    public Button cancelBreak, finishBreak, cancelExhibit, finishExhibit;
}
