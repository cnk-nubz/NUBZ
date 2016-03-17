package com.cnk.data.map;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.Downloader;
import com.cnk.data.FileHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.ZoomLevelResolution;
import com.cnk.exceptions.DatabaseLoadException;
import com.cnk.utilities.Consts;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class MapData {
    public interface MapUpdateAction {
        void doOnUpdate();
    }
    private static final String LOG_TAG = "MapData";
    private static final String MAP_DIRECTORY = "maps";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String TMP = "TMP";
    private static MapData instance;
    private DatabaseHelper dbHelper;
    private List<FloorMapInfo> floorInfos;

    private MapData() {
        floorInfos = new ArrayList<>();
    }

    public static MapData getInstance() {
        if (instance == null) {
            instance = new MapData();
        }
        return instance;
    }

    public void downloadMap(MapUpdateAction action) {
        NetworkHandler.getInstance().downloadMap(action);
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    public void loadDbData() throws DatabaseLoadException {
        try {
            loadMapFromDb();
        } catch (Exception e) {
            e.printStackTrace();
            throw new DatabaseLoadException();
        }
    }

    private void loadMapFromDb() throws DatabaseLoadException {
        floorInfos.clear();
        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            Integer zoomLevelsCount = dbHelper.getZoomLevelsCount(floor);
            if (zoomLevelsCount == 0) {
                throw new DatabaseLoadException();
            }

            ArrayList<ZoomLevelResolution> zoomLevelsResolutions = new ArrayList<>();
            ArrayList<MapTileInfo> mapTileInfos = new ArrayList<>();
            for (int zoomLevel = 0; zoomLevel < zoomLevelsCount; zoomLevel++) {
                zoomLevelsResolutions.add(dbHelper.getZoomLevelResolution(floor, zoomLevel));
                mapTileInfos.add(dbHelper.getMapTileInfo(floor, zoomLevel));
            }

            FloorMapInfo currentFloor = new FloorMapInfo();
            currentFloor.setZoomLevelsResolutions(zoomLevelsResolutions);
            currentFloor.setMapTilesSizes(mapTileInfos);
            floorInfos.add(currentFloor);
        }
    }

    public void setMaps(List<FloorMap> maps) throws IOException {
        Log.i(LOG_TAG, "Setting new maps");
        for (FloorMap map : maps) {
            downloadAndSaveFloor(map);
        }

        FileHandler.getInstance().renameFile(Consts.DATA_PATH + MAP_DIRECTORY + TMP, MAP_DIRECTORY);
        Log.i(LOG_TAG, "Files saved, saving to db");

        for (FloorMap map : maps) {
            dbHelper.setMap(map);
        }
        loadDbData();
        Log.i(LOG_TAG, "New maps set");
    }

    private void downloadAndSaveFloor(FloorMap map) throws IOException {
        ArrayList<ZoomLevel> levels = map.getZoomLevels();
        for (int level = 0; level < levels.size(); level++) {
            ArrayList<ArrayList<String>> tiles = levels.get(level).getTilesFiles();
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles.get(i).size(); j++) {
                    InputStream in = Downloader.getInstance().download(tiles.get(i).get(j));
                    String tmpFilename = getTemporaryPathForTile(map.getFloor(), level, i, j);
                    String actualFilename = getPathForTile(map.getFloor(), level, i, j);
                    FileHandler.getInstance().saveInputStream(in, tmpFilename);
                    tiles.get(i).set(j, actualFilename);
                }
            }
        }
    }

    private String getTemporaryPathForTile(Integer floorNo,
                                           Integer zoomLevel,
                                           Integer x,
                                           Integer y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY + TMP + "/";
        return dir + getTileFilename(x, y, floorNo, zoomLevel);
    }

    private String getPathForTile(Integer floorNo, Integer zoomLevel, Integer x, Integer y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY;
        return dir + getTileFilename(x, y, floorNo, zoomLevel);
    }

    public Bitmap getTile(Integer floor, Integer zoomLevel, Integer row, Integer column) {
        String tileFilename = getTileFilename(row, column, floor, zoomLevel);
        tileFilename = Consts.DATA_PATH + MAP_DIRECTORY + "/" + tileFilename;
        Bitmap bmp = null;
        InputStream in = null;
        try {
            in = FileHandler.getInstance().getFile(tileFilename);
            bmp = BitmapFactory.decodeStream(in);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "File " + tileFilename + " not found");
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return bmp;
    }

    private String getTileFilename(Integer x, Integer y, Integer floorNo, Integer zoomLevel) {
        return TILE_FILE_PREFIX + floorNo.toString() + "_" + zoomLevel.toString() + "_" +
               x.toString() + "_" + y.toString();
    }

    public Boolean mapForFloorExists(Integer floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().size() > 0;
    }

    public Integer getZoomLevelsCount(Integer floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().size();
    }

    public Resolution getOriginalResolution(Integer floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().get(0).getOriginalRes();
    }

    public ZoomLevelResolution getZoomLevelResolution(Integer floor, Integer zoomLevel) {
        return floorInfos.get(floor).getZoomLevelsResolutions().get(zoomLevel);
    }

    public Resolution getTileSize(Integer floor, Integer zoomLevel) {
        MapTileInfo size = floorInfos.get(floor).getMapTilesSizes().get(zoomLevel);
        return size != null ? size.getTileSize() : null;
    }
}