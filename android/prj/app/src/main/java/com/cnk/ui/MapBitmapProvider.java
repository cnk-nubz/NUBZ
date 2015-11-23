package com.cnk.ui;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import com.qozix.tileview.graphics.BitmapProvider;
import com.qozix.tileview.tiles.Tile;

public class MapBitmapProvider implements BitmapProvider {

    private Bitmap tileImages[][];

    private Boolean tilesReady;

    public MapBitmapProvider() {
        tilesReady = false;
    }

    public void prepareTiles(Bitmap mapBitmap) {
        if (tilesReady) {
            return;
        }

        int columns = mapBitmap.getWidth() / 256;
        if (mapBitmap.getWidth() % 256 != 0) {
            columns++;
        }

        int rows = mapBitmap.getHeight() / 256;
        if (mapBitmap.getHeight() % 256 != 0) {
            rows++;
        }

        tileImages = new Bitmap[columns + 1][rows + 1];
        for (int col = 0; col < columns; col++) {
            for (int row = 0; row < rows; row++) {
                tileImages[col][row] = Bitmap.createBitmap(mapBitmap, col * 256, row * 256,
                        Math.min(256, mapBitmap.getWidth() - col * 256),
                        Math.min(256, mapBitmap.getHeight() - row * 256));
            }
        }

        tilesReady = true;
    }

    public Boolean isReady() {
        return tilesReady;
    }

    @Override
    public Bitmap getBitmap(Tile tile, Context context) {
        if (!tilesReady) {
            return null;
        } else {
            return tileImages[tile.getColumn()][tile.getRow()];
        }
    }
}
