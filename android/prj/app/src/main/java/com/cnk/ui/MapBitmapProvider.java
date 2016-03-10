package com.cnk.ui;

import android.content.Context;
import android.graphics.Bitmap;

import com.cnk.data.DataHandler;
import com.qozix.tileview.graphics.BitmapProvider;
import com.qozix.tileview.tiles.Tile;

public class MapBitmapProvider implements BitmapProvider {

    private Integer floor;

    public MapBitmapProvider(Integer floor) {
        this.floor = floor;
    }

    @Override
    public Bitmap getBitmap(Tile tile, Context context) {
        return DataHandler.getInstance()
                          .getTile(floor,
                                   (Integer) tile.getData(),
                                   tile.getColumn(),
                                   tile.getRow());
    }
}
