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
import com.cnk.notificators.Observable;
import com.cnk.utilities.Consts;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class MapData extends Observable<MapData.MapDownloadUpdateAction> {
    public interface MapDownloadUpdateAction {
        void doOnUpdate(int downloaded, int allToDownload);
    }

    private static final String LOG_TAG = "MapData";
    private static final String MAP_DIRECTORY = "maps";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String TMP = "TMP";
    private static final Integer TILE_DOWNLOAD_RETRYS = 3;
    private static final Integer TILE_DOWNLOAD_FAILURE_WAIT_SECONDS = 3;
    private static MapData instance;
    private DatabaseHelper dbHelper;
    private List<FloorMapInfo> floorInfos;
    private Integer floors;

    private MapData() {
        floorInfos = new ArrayList<>();
    }

    public static MapData getInstance() {
        if (instance == null) {
            instance = new MapData();
        }
        return instance;
    }

    public void notifyObservers(int downloaded, int allToDownload) {
        List<MapDownloadUpdateAction> actions = new ArrayList<>();
        for (MapDownloadUpdateAction action : observers.values()) {
            actions.add(action);
        }

        for (MapDownloadUpdateAction action : actions) {
            action.doOnUpdate(downloaded, allToDownload);
        }
    }

    public void downloadMap(NetworkHandler.SuccessAction success,
                            NetworkHandler.FailureAction failure) {
        NetworkHandler.getInstance().downloadMap(success, failure);
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
        floors = dbHelper.getFloorCount();
        if (floors == null) {
            throw new DatabaseLoadException();
        }
        for (int floor = 0; floor < floors; floor++) {
            int zoomLevelsCount = dbHelper.getZoomLevelsCount(floor);
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
        dbHelper.clearMaps();
        int allTilesCount = 0;
        int downloadedTilesCount = 0;
        for (FloorMap map : maps) {
            ArrayList<ZoomLevel> zoomLevels = map.getZoomLevels();
            for (ZoomLevel level : zoomLevels) {
                ArrayList<ArrayList<String>> tiles = level.getTilesFiles();
                for (ArrayList<String> tilesRow : tiles) {
                    allTilesCount += tilesRow.size();
                }
            }
        }

        Log.i(LOG_TAG, "Setting new maps");
        for (FloorMap map : maps) {
            downloadedTilesCount += downloadAndSaveFloor(map, allTilesCount, downloadedTilesCount);
        }

        FileHandler.getInstance().renameFile(Consts.DATA_PATH + MAP_DIRECTORY + TMP, MAP_DIRECTORY);
        Log.i(LOG_TAG, "Files saved, saving to db");

        for (FloorMap map : maps) {
            dbHelper.setMap(map);
        }
        loadDbData();
        Log.i(LOG_TAG, "New maps set");
    }

    private int downloadAndSaveFloor(FloorMap map,
                                     int allTiles,
                                     int alreadyDownloadedTiles) throws IOException {
        int tilesCountFromFloor = 0;
        ArrayList<ZoomLevel> levels = map.getZoomLevels();
        for (int level = 0; level < levels.size(); level++) {
            ArrayList<ArrayList<String>> tiles = levels.get(level).getTilesFiles();
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles.get(i).size(); j++) {
                    String tmpFilename = getTemporaryPathForTile(map.getFloor(), level, i, j);
                    String actualFilename = getPathForTile(map.getFloor(), level, i, j);

                    int errors = 0;
                    while (errors++ < TILE_DOWNLOAD_RETRYS) {
                        try {
                            InputStream in = Downloader.getInstance().download(tiles.get(i).get(j));
                            FileHandler.getInstance().saveInputStream(in, tmpFilename);
                            break;
                        } catch (IOException e) {
                            if (errors < TILE_DOWNLOAD_RETRYS) {
                                Log.i(LOG_TAG,
                                      "Downloading " + tiles.get(i).get(j) + " failed, retrying.");
                                try {
                                    Thread.sleep(TILE_DOWNLOAD_FAILURE_WAIT_SECONDS *
                                                 Consts.MILLIS_IN_SEC);
                                } catch (InterruptedException e1) {
                                    e1.printStackTrace();
                                }
                            } else {
                                Log.e(LOG_TAG, "Map tile downloading failed after " +
                                               TILE_DOWNLOAD_RETRYS.toString() +
                                               " retrys.");
                                throw e;
                            }
                        }
                    }

                    tiles.get(i).set(j, actualFilename);
                    tilesCountFromFloor++;
                    notifyObservers(alreadyDownloadedTiles + tilesCountFromFloor, allTiles);
                }
            }
        }
        return tilesCountFromFloor;
    }

    private String getTemporaryPathForTile(int floorNo, int zoomLevel, int x, int y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY + TMP + "/";
        return dir + getTileFilename(x, y, floorNo, zoomLevel);
    }

    private String getPathForTile(int floorNo, int zoomLevel, int x, int y) {
        String dir = Consts.DATA_PATH + MAP_DIRECTORY;
        return dir + getTileFilename(x, y, floorNo, zoomLevel);
    }

    public Bitmap getTile(int floor, int zoomLevel, int row, int column) {
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

    public Integer getFloors() {
        Log.i(LOG_TAG, floors.toString());
        return floors;
    }

    private String getTileFilename(int x, int y, int floorNo, int zoomLevel) {
        return TILE_FILE_PREFIX + floorNo + "_" + zoomLevel + "_" + x + "_" + y;
    }

    public Boolean mapForFloorExists(int floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().size() > 0;
    }

    public List<ZoomLevelInfo> getZoomLevels(int floor) {
        int count = getZoomLevelsCount(floor);
        List<ZoomLevelInfo> res = new ArrayList<>();
        for (int i = 0; i < count; i++) {
            ZoomLevelInfo info = new ZoomLevelInfo();
            info.tileSize = getTileSize(floor, i);
            info.scaledSize = getZoomLevelResolution(floor, i).getScaledRes();
            res.add(info);
        }
        return res;
    }

    public int getZoomLevelsCount(int floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().size();
    }

    public Resolution getOriginalResolution(int floor) {
        return floorInfos.get(floor).getZoomLevelsResolutions().get(0).getOriginalRes();
    }

    public Resolution getMaxZoomResolution(int floor) {
        int maxZoomLevel = getZoomLevelsCount(floor) - 1;
        return floorInfos.get(floor).getZoomLevelsResolutions().get(maxZoomLevel).getScaledRes();
    }

    public ZoomLevelResolution getZoomLevelResolution(int floor, int zoomLevel) {
        return floorInfos.get(floor).getZoomLevelsResolutions().get(zoomLevel);
    }

    public Resolution getTileSize(int floor, int zoomLevel) {
        MapTileInfo size = floorInfos.get(floor).getMapTilesSizes().get(zoomLevel);
        return size != null ? size.getTileSize() : null;
    }
}