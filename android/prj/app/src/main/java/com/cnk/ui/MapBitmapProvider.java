package com.cnk.ui;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Pair;

import com.qozix.tileview.graphics.BitmapProvider;
import com.qozix.tileview.tiles.Tile;

public class MapBitmapProvider implements BitmapProvider {

    private Bitmap tileImages[][], tileImagesInterpolated[][];

    private Boolean tilesReady;

    public MapBitmapProvider() {
        tilesReady = false;
    }

    public void prepareTiles(Bitmap mapBitmap) {
        if (tilesReady) {
            return;
        }

        Pair<Integer, Integer> newDimensions = ScaleHelper.getInterpolatedDimensions(mapBitmap.getWidth(), mapBitmap.getHeight());

        System.gc();
        Bitmap interpolated = Bitmap.createScaledBitmap(mapBitmap, newDimensions.first, newDimensions.second, true);
        mapBitmap.recycle();

        int columns = interpolated.getWidth() / 256;
        if (interpolated.getWidth() % 256 != 0) {
            columns++;
        }

        int rows = interpolated.getHeight() / 256;
        if (interpolated.getHeight() % 256 != 0) {
            rows++;
        }

        tileImagesInterpolated = new Bitmap[columns + 1][rows + 1];
        for (int col = 0; col < columns; col++) {
            for (int row = 0; row < rows; row++) {
                tileImagesInterpolated[col][row] = Bitmap.createBitmap(interpolated, col * 256, row * 256,
                        Math.min(256, interpolated.getWidth() - col * 256),
                        Math.min(256, interpolated.getHeight() - row * 256));
            }
        }
        interpolated.recycle();
        System.gc();

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
            return tileImagesInterpolated[tile.getColumn()][tile.getRow()];
        }
    }
}
