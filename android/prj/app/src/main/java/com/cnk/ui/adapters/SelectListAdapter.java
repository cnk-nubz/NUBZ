package com.cnk.ui.adapters;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.ListObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SelectListAdapter<T extends ListObject> extends BaseAdapter {
    private class OptionOnClickListener implements View.OnClickListener {
        private Integer optionId;
        private SelectListAdapter<T> adapterRef;

        public OptionOnClickListener(SelectListAdapter<T> adapterRef, Integer optionId) {
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
    private static final Integer ACTIVE_COLOR = Color.GREEN;
    private static final Integer NOT_ACTIVE_COLOR = 0xffd3d3d3;
    private static final Integer DEFAULT_LAYOUT = R.layout.select_list_item_default;
    private List<T> options;
    private Context context;
    private Boolean optionChecked[];
    private Boolean singleAnswer;
    private View activeView;
    private Integer activeId;
    private Integer layout;
    private SelectViewItemIds viewIds;

    public SelectListAdapter(List<T> options, Boolean singleAnswer, Context context) {
        this(options, singleAnswer, context, DEFAULT_LAYOUT, new SelectViewItemIds());
    }

    public SelectListAdapter(List<T> options,
                             Boolean singleAnswer,
                             Context context,
                             Integer layout,
                             SelectViewItemIds viewIds) {
        this.options = options;
        this.context = context;
        this.optionChecked = new Boolean[options.size()];
        this.singleAnswer = singleAnswer;
        this.layout = layout;
        this.viewIds = viewIds;
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
        LayoutInflater
                inflater =
                (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View optionLabel = inflater.inflate(layout, null);
        TextView text = (TextView) optionLabel.findViewById(viewIds.getTextViewId());
        if (text != null) {
            text.setText(options.get(idx).getText());
        }
        if (isSelected(idx)) {
            optionLabel.setBackgroundColor(ACTIVE_COLOR);
            activeId = idx;
            activeView = optionLabel;
        } else {
            optionLabel.setBackgroundColor(NOT_ACTIVE_COLOR);
        }
        optionLabel.setOnClickListener(new OptionOnClickListener(this, idx));
        return optionLabel;
    }

    private Boolean isSelected(Integer id) {
        return optionChecked[id];
    }

    private void selectOption(Integer id, View v) {
        if (activeId != null && activeView != null && singleAnswer) {
            unselectOption(activeId, activeView);
        }
        optionChecked[id] = true;
        v.setBackgroundColor(ACTIVE_COLOR);
        activeId = id;
        activeView = v;
    }

    private void unselectOption(Integer id, View v) {
        optionChecked[id] = false;
        v.setBackgroundColor(NOT_ACTIVE_COLOR);
        activeId = null;
        activeView = null;
    }

    public void unselectAllOptions() {
        Arrays.fill(optionChecked, Boolean.FALSE);
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
}
