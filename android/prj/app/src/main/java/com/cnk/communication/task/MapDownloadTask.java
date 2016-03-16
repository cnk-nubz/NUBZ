package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.thrift.MapImage;
import com.cnk.communication.thrift.NewMapImagesRequest;
import com.cnk.communication.thrift.NewMapImagesResponse;
import com.cnk.communication.thrift.Server;
import com.cnk.communication.thrift.Size;
import com.cnk.communication.thrift.ZoomLevel;
import com.cnk.data.map.FloorMap;
import com.cnk.data.map.MapData;
import com.cnk.data.map.MapTiles;
import com.cnk.data.map.Resolution;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Consts;

import org.apache.thrift.TException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MapDownloadTask extends ServerTask {

    private static final String LOG_TAG = "MapDownloadTask";
    private MapData.MapUpdateAction action;


    public MapDownloadTask(Notificator notificator, MapData.MapUpdateAction action) {
        super(notificator);
        this.action = action;
    }

    public void performInSession(Server.Client client) throws TException, IOException {
        Integer version = MapData.getInstance().getMapVersion();
        downloadTilesUpdate(client, version);
        Log.i(LOG_TAG, "Map update complete");
        action.doOnUpdate();
        action = null;
    }

    private void downloadTilesUpdate(Server.Client client,
                                     Integer version) throws TException, IOException {
        Log.i(LOG_TAG, "Downloading map tiles addresses");

        NewMapImagesRequest request = new NewMapImagesRequest();
        if (version != null) {
            request.setAcquiredVersion(version);
        }

        NewMapImagesResponse response = client.getNewMapImages(request);
        MapImage floor1Response = response.getFloors().get(Consts.FLOOR1);
        MapImage floor2Response = response.getFloors().get(Consts.FLOOR2);

        Log.i(LOG_TAG, "Map tiles addresses downloaded");
        FloorMap floor1 = translateFromThrift(floor1Response);
        FloorMap floor2 = translateFromThrift(floor2Response);
        MapData.getInstance().setMaps(version, floor1, floor2);
    }

    private FloorMap translateFromThrift(MapImage thriftResponse) {
        if (thriftResponse == null) {
            return null;
        }
        Size thriftSize = thriftResponse.getResolution();
        Resolution originalSize = new Resolution(thriftSize.getWidth(), thriftSize.getHeight());
        List<ZoomLevel> imageTilesThrift = thriftResponse.getZoomLevels();
        ArrayList<MapTiles> mapTiles = new ArrayList<>();

        for (ZoomLevel tile : imageTilesThrift) {
            Resolution
                    scaledSize =
                    new Resolution(tile.getScaledSize().getWidth(),
                                   tile.getScaledSize().getHeight());
            Resolution
                    tileSize =
                    new Resolution(tile.getTileSize().getWidth(), tile.getTileSize().getHeight());
            List<List<String>> toCopy = tile.getTilesUrls();
            MapTiles toAdd = new MapTiles(scaledSize, tileSize, copyThriftList(toCopy));
            mapTiles.add(toAdd);
        }

        return new FloorMap(originalSize, mapTiles);
    }

    private ArrayList<ArrayList<String>> copyThriftList(List<List<String>> thriftList) {
        ArrayList<ArrayList<String>> copied = new ArrayList<>();
        for (List<String> list : thriftList) {
            ArrayList<String> toAdd = new ArrayList<String>();
            for (String s : list) {
                toAdd.add(s);
            }
            copied.add(toAdd);
        }
        return copied;
    }
}
