package com.cnk.ui;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.util.Pair;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class ImageHelper {
    public static final Integer maximumPixCount = 14 * 1000 * 1000;
    public static final String ScaledMapPrefix = "scaled_map_";
    public static final String LOG_TAG = "ImageHelper";
    private static final Integer floor0code = 0;
    private static final Integer floor1code = 1;
    private static final Integer maximumZoom = 4;
    private static final float expectedMaximumExhibitToScreenAreaRatio = 0.001f;

    private static Pair<Integer, Integer> getInterpolatedDimensions(Integer width, Integer height, Integer minExhibitSide, Integer screenPixelsCount) {
        if (minExhibitSide == null) {
            Log.i(LOG_TAG, "No exhibit data, default scale.");
            return new Pair<>(width / 2, height / 2);
        }

        Integer imagePixels = width * height;
        Integer smallestPossibleExhibitPixels = minExhibitSide * minExhibitSide;

        Float scale = (screenPixelsCount * expectedMaximumExhibitToScreenAreaRatio) / smallestPossibleExhibitPixels * maximumZoom;
        Log.i(LOG_TAG, "screen pixels count: " + Integer.toString(screenPixelsCount));
        Log.i(LOG_TAG, "smallest possible exhibit pixels: " + smallestPossibleExhibitPixels.toString());

        Log.i(LOG_TAG, "Width after scaling: " + Integer.toString((int) (width * scale))
                + " Height after scaling: " + Integer.toString((int) (height * scale)));

        if ((int) (width * scale * height * scale) > maximumPixCount) {
            Integer midWidth = 0, midHeight = 0;
            Integer lowerWidth = (int) (width * scale / 10), higherWidth = width;
            Integer lowerHeight = (int) (height * scale / 10), higherHeight = height;

            while (midWidth != (lowerWidth + higherWidth) / 2) {
                midWidth = (lowerWidth + higherWidth) / 2;
                midHeight = (lowerHeight + higherHeight) / 2;

                if (midWidth * midHeight <= maximumPixCount) {
                    lowerWidth = midWidth;
                    lowerHeight = midHeight;
                } else {
                    higherWidth = midWidth;
                    higherHeight = midHeight;
                }
            }
            Log.i(LOG_TAG, "Since calculated dimensions exceeded maximum resolution, new " +
                    "dimensions were calculated, width: " + lowerWidth.toString() +
                    " height: " + lowerHeight.toString());
            return new Pair<>(lowerWidth, lowerHeight);
        } else {
            return new Pair<>((int) (width * Math.sqrt(scale)), (int) (height * Math.sqrt(scale)));
        }
    }

    public static Bitmap bitmapFromDrawable(Drawable drawable) {
        Bitmap map = null;

        if (drawable instanceof BitmapDrawable) {
            BitmapDrawable bitmapDrawable = (BitmapDrawable) drawable;
            map = bitmapDrawable.getBitmap();
        }

        if (map == null) {
            map = Bitmap.createBitmap(drawable.getIntrinsicWidth(),
                    drawable.getIntrinsicHeight(), Bitmap.Config.RGB_565);
            Canvas canvas = new Canvas(map);
            Drawable temporary = drawable.getCurrent();
            temporary.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            temporary.draw(canvas);
        }

        return map;
    }

    public static Integer getDimensionWhenScaleApplied(Integer dimensionToScale, Integer exampleOriginalValue, Integer exampleScaled) {
        Float scale = (float) exampleScaled / exampleOriginalValue;
        return (int) (dimensionToScale * scale);
    }

    public static Bitmap scaleBitmapToProperSize(Bitmap toScale, Integer minExhibitSide, Integer screenRes) {
        Pair<Integer, Integer> newDimensions = ImageHelper.getInterpolatedDimensions(toScale.getWidth(),
                toScale.getHeight(), minExhibitSide, screenRes);

        System.gc();
        Log.i(LOG_TAG, "creating scaled bitmap begin");
        Bitmap result = Bitmap.createScaledBitmap(toScale, newDimensions.first, newDimensions.second, true);
        Log.i(LOG_TAG, "creating scaled bitmap end");

        return result;
    }

    public static void saveScaledBitmapForFloor(Context context, Bitmap bmp, Integer floor) throws IOException {
        try {
            FileOutputStream fos = context.openFileOutput(ScaledMapPrefix + floor.toString(), Context.MODE_PRIVATE);
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.close();
            Log.i(LOG_TAG, "Scaled map saved to file " + ScaledMapPrefix + floor.toString());
        } catch (IOException ioe) {
            ioe.printStackTrace();
            Log.i(LOG_TAG, "Saving scaled map file failed");
            throw ioe;
        }
    }

    public static Bitmap getScaledMapBitmapForFloor(Context context, Integer floor) throws FileNotFoundException {
        try {
            FileInputStream fis = context.openFileInput(ScaledMapPrefix + Integer.toString(floor));
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inPreferredConfig = Bitmap.Config.RGB_565;
            return BitmapFactory.decodeStream(fis, null, options);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            throw e;
        }
    }

    public void removeScaledBitmaps(Context context) {
        context.deleteFile(ScaledMapPrefix + Integer.toString(floor0code));
        context.deleteFile(ScaledMapPrefix + Integer.toString(floor1code));
    }

}
