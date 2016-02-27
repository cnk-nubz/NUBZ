package com.cnk.utilities;

import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SelectListAdapter<T extends ListObject, V extends TextView> extends BaseAdapter {
    private static final Integer ACTIVE_COLOR = Color.GREEN;
    private static final Integer NOTACTIVE_COLOR = Color.LTGRAY;

    private List<T> options;
    private Context context;
    private Boolean optionChecked[];
    private Boolean singleAnswer;
    private View activeView;
    private Integer activeId;
    private Class<V> viewType;

    public SelectListAdapter(List<T> options,
                             Boolean singleAnswer,
                             Context context,
                             Class<V> viewType
    ) {
        this.options = options;
        this.context = context;
        this.optionChecked = new Boolean[options.size()];
        this.singleAnswer = singleAnswer;
        this.viewType = viewType;
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
    public View getView(final int idx, View view, ViewGroup viewGroup) {
        V optionLabel;
        try {
            optionLabel = viewType.getConstructor(Context.class).newInstance(context);
        } catch (Exception e) {
            throw new RuntimeException();
        }
        optionLabel.setMaxLines(3);
        optionLabel.setTextSize(20f);
        optionLabel.setHeight(80);
        optionLabel.setGravity(Gravity.CENTER);
        optionLabel.setBackgroundColor(NOTACTIVE_COLOR);
        optionLabel.setText(options.get(idx).getText());
        optionLabel.setOnClickListener(new OptionOnClickListener(this, idx));
        return optionLabel;
    }

    private Boolean isSelected(Integer id) {
        return optionChecked[id];
    }

    private void selectOption(Integer id, View v) {
        v.setBackgroundColor(ACTIVE_COLOR);
        optionChecked[id] = true;
        if (activeId != null && activeView != null && singleAnswer) {
            unselectOption(activeId, activeView);
        }
        activeId = id;
        activeView = v;
    }

    private void unselectOption(Integer id, View v) {
        v.setBackgroundColor(NOTACTIVE_COLOR);
        optionChecked[id] = false;
        activeId = null;
        activeView = null;
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
        private SelectListAdapter<T, V> adapterRef;

        public OptionOnClickListener(SelectListAdapter<T, V> adapterRef, Integer optionId) {
            this.optionId = optionId;
            this.adapterRef = adapterRef;
        }

        @Override
        public void onClick(View view) {
            if (isSelected(optionId)) {
                unselectOption(optionId, view);
            } else {
                adapterRef.selectOption(optionId, view);
            }
        }
    }
}
