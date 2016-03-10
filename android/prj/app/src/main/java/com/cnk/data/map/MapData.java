package com.cnk.data.map;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.cnk.data.Downloader;
import com.cnk.data.FileHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.Version;
import com.cnk.exceptions.DatabaseLoadException;
import com.cnk.utilities.Consts;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Observable;

public class MapData extends Observable {
    private static final String LOG_TAG = "MapData";
    private static final String MAP_DIRECTORY = "maps";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String TMP = "TMP";
    private static MapData instance;
    Integer mapVersion;
    private DatabaseHelper dbHelper;
    private List<FloorMapInfo> floorInfos;

    private MapData() {
        floorInfos = new ArrayList<>();
        mapVersion = null;
    }

    public static MapData getInstance() {
        if (instance == null) {
            instance = new MapData();
        }
        return instance;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    public void loadDbData() throws DatabaseLoadException {
        mapVersion = dbHelper.getVersion(Version.Item.MAP);
        try {
            loadMapFromDb();
        } catch (Exception e) {
            e.printStackTrace();
            throw new DatabaseLoadException();
        }
    }

    private void loadMapFromDb() {
        floorInfos.clear();
        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            FloorMapInfo currentFloor = new FloorMapInfo();
            currentFloor.setDetailLevelsCount(dbHelper.getDetailLevelsForFloor(floor));
            ArrayList<DetailLevelRes> detailLevelRes = new ArrayList<>();
            ArrayList<MapTileInfo> mapTileInfos = new ArrayList<>();
            for (int detailLevel = 0;
                 detailLevel < currentFloor.getDetailLevelsCount();
                 detailLevel++) {
                detailLevelRes.add(dbHelper.getDetailLevelRes(floor, detailLevel));
                mapTileInfos.add(dbHelper.getMapTileInfo(floor, detailLevel));
            }
            currentFloor.setDetailLevelRes(detailLevelRes);
            currentFloor.setMapTilesSizes(mapTileInfos);
            floorInfos.add(currentFloor);
        }
    }

    public void setMaps(Integer version,
                        FloorMap floor0,
                        FloorMap floor1) throws IOException, DatabaseLoadException {
        Log.i(LOG_TAG, "Setting new maps");
        Boolean floor0Changed = downloadAndSaveFloor(floor0, Consts.FLOOR1);
        Boolean floor1Changed = downloadAndSaveFloor(floor1, Consts.FLOOR2);
        if (floor0Changed || floor1Changed) {
            FileHandler.getInstance()
                       .renameFile(Consts.DATA_PATH + MAP_DIRECTORY + TMP, MAP_DIRECTORY);
        }
        Log.i(LOG_TAG, "Files saved, saving to db");
        dbHelper.setMaps(version, floor0, floor1);
        loadDbData();
        setChanged();
        notifyMapUpdated();

        Log.i(LOG_TAG, "New maps set");
    }

    public void notifyMapUpdated() {
        setChanged();
        notifyObservers();
    }

    public Bitmap getTile(Integer floor, Integer detailLevel, Integer row, Integer column) {
        String tileFilename = getTileFilename(row, column, floor, detailLevel);
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

    public Boolean mapForFloorExists(Integer floor) {
        return floorInfos.get(floor).getDetailLevelsCount() > 0;
    }

    public Integer getDetailLevelsCountForFloor(Integer floor) {
        return floorInfos.get(floor).getDetailLevelsCount();
    }

    public Resolution getOriginalResolution(Integer floor) {
        return floorInfos.get(floor).getDetailLevelRes().get(0).getOriginalRes();
    }

    public Integer getMapVersion() {
        return mapVersion;
    }

    public DetailLevelRes getDetailLevelResolution(Integer floor, Integer detailLevel) {
        return floorInfos.get(floor).getDetailLevelRes().get(detailLevel);
    }

    public Resolution getTileSize(Integer floor, Integer detailLevel) {
        MapTileInfo size = floorInfos.get(floor).getMapTilesSizes().get(detailLevel);
        return size != null ? size.getTileSize() : null;
    }

    public String getPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY;
        return dir + getTileFilename(x, y, floorNo, detailLevel);
    }

    public String getTemporaryPathForTile(Integer floorNo,
                                          Integer detailLevel,
                                          Integer x,
                                          Integer y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY + TMP + "/";
        return dir + getTileFilename(x, y, floorNo, detailLevel);
    }

    private String getTileFilename(Integer x, Integer y, Integer floorNo, Integer detailLevel) {
        return TILE_FILE_PREFIX + floorNo.toString() + "_" + detailLevel.toString() + "_" +
               x.toString() + "_" + y.toString();
    }

    private Boolean downloadAndSaveFloor(FloorMap floor, Integer floorNo) throws IOException {
        if (floor == null) {
            return false;
        }

        ArrayList<MapTiles> levels = floor.getLevels();
        for (int level = 0; level < levels.size(); level++) {
            ArrayList<ArrayList<String>> tiles = levels.get(level).getTilesFiles();
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles.get(i).size(); j++) {
                    InputStream in = Downloader.getInstance().download(tiles.get(i).get(j));
                    String tmpFilename = getTemporaryPathForTile(floorNo, level, i, j);
                    String actualFilename = getPathForTile(floorNo, level, i, j);
                    FileHandler.getInstance().saveInputStream(in, tmpFilename);
                    tiles.get(i).set(j, actualFilename);
                }
            }
        }
        return true;
    }
}
