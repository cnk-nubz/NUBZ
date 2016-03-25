package com.cnk.activities.map;

import android.content.Context;
import android.graphics.Bitmap;
import android.support.annotation.NonNull;

import com.cnk.data.map.MapData;
import com.qozix.tileview.graphics.BitmapProvider;
import com.qozix.tileview.tiles.Tile;

public class MapBitmapProvider implements BitmapProvider {

    private int floor;

    public MapBitmapProvider(int floor) {
        this.floor = floor;
    }

    @Override
    public Bitmap getBitmap(@NonNull Tile tile, @NonNull Context context) {
        return MapData.getInstance()
                      .getTile(floor, (Integer) tile.getData(), tile.getColumn(), tile.getRow());
    }
}