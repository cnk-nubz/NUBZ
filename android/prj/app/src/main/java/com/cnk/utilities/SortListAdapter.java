package com.cnk.utilities;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;

import com.cnk.R;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class SortListAdapter<T extends ListObject> extends BaseAdapter {
    private static final Integer DEFAULT_LAYOUT = R.layout.sort_list_item_default;

    private List<T> options;
    private Context context;
    private Integer layout;
    private Integer mainViewId;
    private Integer textId;
    SortItemIds ids;

    public SortListAdapter(List<T> options, Context context) {
        this(options, context, DEFAULT_LAYOUT, new SortItemIds());
    }

    public SortListAdapter(List<T> options, Context context, Integer layout) {
        this(options, context, layout, new SortItemIds());
    }

    public SortListAdapter(List<T> options, Context context, Integer layout, SortItemIds ids) {
        this.options = options;
        this.context = context;
        this.layout = layout;
        this.ids = ids;
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
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View option = inflater.inflate(layout, null);
        TextView text = (TextView) option.findViewById(ids.getTextView());
        ImageButton downButon = (ImageButton) option.findViewById(ids.getDownButton());
        ImageButton upButton = (ImageButton) option.findViewById(ids.getUpButton());
        if (upButton != null) {
            upButton.setOnClickListener(new UpClickListener(idx));
        }
        if (downButon != null) {
            downButon.setOnClickListener(new DownClickListener(idx));
        }
        if (text != null) {
            text.setText(options.get(idx).getText());
        }
        return option;
    }

    public List<Integer> getOrder() {
        List<Integer> order = new ArrayList<>();
        for (T item : options) {
            order.add(item.getId());
        }
        return order;
    }

    private class DownClickListener implements ImageButton.OnClickListener {
        private Integer idx;
        public DownClickListener(Integer idx) {
            super();
            this.idx = idx;
        }
        @Override
        public void onClick(View v) {
            if (idx < getCount() - 1) {
                Collections.swap(options, idx, idx + 1);
                notifyDataSetChanged();
            }
        }
    }

    private class UpClickListener implements ImageButton.OnClickListener {
        private Integer idx;
        public UpClickListener(Integer idx) {
            super();
            this.idx = idx;
        }
        @Override
        public void onClick(View v) {
            if (idx > 0) {
                Collections.swap(options, idx, idx - 1);
                notifyDataSetChanged();
            }
        }
    }
}
