package com.cnk.utilities;

import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import com.cnk.ui.AutoResizeTextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SelectListAdapter<T extends IdTextObject> extends BaseAdapter {
    private List<T> options;
    private Context context;
    private Boolean optionChecked[];
    private Boolean singleAnswer;
    private View activeView;
    private Integer activeId;

    public SelectListAdapter(List<T> options,
                                        Boolean singleAnswer,
                                        Context context) {
        this.options = options;
        this.context = context;
        this.optionChecked = new Boolean[options.size()];
        this.singleAnswer = singleAnswer;
        activeId = null;
        activeView = null;
        Arrays.fill(optionChecked, false);
    }

    @Override
    public int getCount() {
        return options.size();
    }

    @Override
    public Object getItem(int i) {
        // NOT SUPPORTED
        return null;
    }

    @Override
    public long getItemId(int i) {
        // NOT SUPPORTED
        return 0;
    }

    @Override
    public View getView(final int i, View view, ViewGroup viewGroup) {
        AutoResizeTextView optionLabel = new AutoResizeTextView(context);
        optionLabel.setMaxLines(3);
        optionLabel.setMinTextSize(2f);
        optionLabel.setTextSize(20f);
        optionLabel.setWidth(200);
        optionLabel.setHeight(80);
        optionLabel.setGravity(Gravity.CENTER);
        optionLabel.setBackgroundColor(Color.LTGRAY);
        optionLabel.setText(options.get(i).getText());
        optionLabel.setOnClickListener(new OptionOnClickListener(this, i));
        return optionLabel;
    }

    private void selectOption(Integer id, View v) {
        if (activeId != null && activeView != null && singleAnswer) {
            unselectOption(activeId, activeView);
        }
        v.setBackgroundColor(Color.GREEN);
        optionChecked[id] = true;
        activeId = id;
        activeView = v;
    }

    private void unselectOption(Integer id, View v) {
        v.setBackgroundColor(Color.LTGRAY);
        optionChecked[activeId] = false;
    }

    public ArrayList<Integer> getSelectedOptions() {
        ArrayList<Integer> resultActions = new ArrayList<>();
        for (int i = 0; i < options.size(); i++) {
            if (optionChecked[i]) {
                resultActions.add(options.get(i).getId());
            }
        }
        return resultActions;
    }

    private class OptionOnClickListener implements View.OnClickListener {
        private Integer optionId;
        private SelectListAdapter<T> adapterRef;

        public OptionOnClickListener(SelectListAdapter<T> adapterRef, Integer optionId) {
            this.optionId = optionId;
            this.adapterRef = adapterRef;
        }

        @Override
        public void onClick(View view) {
            adapterRef.selectOption(optionId, view);
        }
    }
}
