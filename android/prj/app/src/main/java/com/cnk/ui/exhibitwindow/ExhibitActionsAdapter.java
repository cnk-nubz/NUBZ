package com.cnk.ui.exhibitwindow;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import com.cnk.ui.AutoResizeTextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ExhibitActionsAdapter extends BaseAdapter {
    private Context context;
    private List<String> actionsList;
    private Boolean[] actionsClicked;

    public ExhibitActionsAdapter(Context context, List<String> actionsList) {
        this.context = context;
        this.actionsList = actionsList;
        this.actionsClicked = new Boolean[actionsList.size()];
        Arrays.fill(actionsClicked, Boolean.FALSE);
    }

    @Override
    public int getCount() {
        return actionsList.size();
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int i) {
        return 0;
    }

    @Override
    public View getView(final int i, View view, ViewGroup viewGroup) {
        AutoResizeTextView actionLabel = new AutoResizeTextView(context);
        actionLabel.setMaxLines(3);
        actionLabel.setTextSize(20f);
        actionLabel.setWidth(200);
        actionLabel.setHeight(80);
        actionLabel.setGravity(Gravity.CENTER);
        actionLabel.setBackgroundColor(Color.LTGRAY);

        actionLabel.setText(actionsList.get(i));

        actionLabel.setOnClickListener(new ExhibitActionClickListener(this, i));

        return actionLabel;
    }

    private Boolean isExhibitActionActivated(Integer id) {
        return actionsClicked[id];
    }

    private void changeExhibitActionState(Integer id) {
        Log.d("id: ", Integer.toString(id));
        actionsClicked[id] = !actionsClicked[id];
    }

    public ArrayList<String> getSelectedActions() {
        ArrayList<String> resultActions = new ArrayList<>();
        for (int i = 0; i < actionsList.size(); i++) {
            if (actionsClicked[i]) {
                resultActions.add(actionsList.get(i));
            }
        }

        return resultActions;
    }

    private class ExhibitActionClickListener implements View.OnClickListener {
        private Integer actionId;
        private ExhibitActionsAdapter adapterRef;

        public ExhibitActionClickListener(ExhibitActionsAdapter adapterRef, Integer actionId) {
            this.actionId = actionId;
            this.adapterRef = adapterRef;
        }

        @Override
        public void onClick(View view) {
            adapterRef.changeExhibitActionState(actionId);
            if (adapterRef.isExhibitActionActivated(actionId)) {
                view.setBackgroundColor(Color.GREEN);
            } else {
                view.setBackgroundColor(Color.LTGRAY);
            }
        }
    }
}
