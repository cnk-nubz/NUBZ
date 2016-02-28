package com.cnk.utilities;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.cnk.ui.ListItemStyle;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SelectListAdapter<T extends ListObject, V extends TextView> extends BaseAdapter {
    private List<T> options;
    private Context context;
    private Boolean optionChecked[];
    private Boolean singleAnswer;
    private View activeView;
    private Integer activeId;
    private Class<V> viewType;
    private ListItemStyle style;

    public SelectListAdapter(List<T> options,
                             Boolean singleAnswer,
                             Context context,
                             Class<V> viewType,
                             ListItemStyle style
    ) {
        this.options = options;
        this.context = context;
        this.optionChecked = new Boolean[options.size()];
        this.singleAnswer = singleAnswer;
        this.viewType = viewType;
        this.style = style == null ? new ListItemStyle() : style;
        activeId = null;
        activeView = null;
        Arrays.fill(optionChecked, false);
    }

    public SelectListAdapter(List<T> options,
                             Boolean singleAnswer,
                             Context context,
                             Class<V> viewType
    ) {
        this(options, singleAnswer, context, viewType, null);
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
        style.apply(optionLabel);
        optionLabel.setText(options.get(idx).getText());
        optionLabel.setOnClickListener(new OptionOnClickListener(this, idx));
        return optionLabel;
    }

    private Boolean isSelected(Integer id) {
        return optionChecked[id];
    }

    private void selectOption(Integer id, View v) {
        v.setBackgroundColor(style.getActiveColor());
        optionChecked[id] = true;
        if (activeId != null && activeView != null && singleAnswer) {
            unselectOption(activeId, activeView);
        }
        activeId = id;
        activeView = v;
    }

    private void unselectOption(Integer id, View v) {
        v.setBackgroundColor(style.getNotActiveColor());
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
