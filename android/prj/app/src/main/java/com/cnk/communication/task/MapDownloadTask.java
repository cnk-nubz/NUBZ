package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.ImageTiles;
import com.cnk.communication.MapImageTilesRequest;
import com.cnk.communication.MapImageTilesResponse;
import com.cnk.communication.MapImagesRequest;
import com.cnk.communication.MapImagesResponse;
import com.cnk.communication.Server;
import com.cnk.communication.Size;
import com.cnk.data.DataHandler;
import com.cnk.data.FloorMap;
import com.cnk.data.MapTiles;
import com.cnk.data.Resolution;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Consts;

import org.apache.thrift.TException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class MapDownloadTask extends ServerTask {

    private static final String LOG_TAG = "MapDownloadTask";


    public MapDownloadTask(Notificator notificator) {
        super(notificator);
    }

    public void performInSession(Server.Client client) throws TException, IOException {
        MapImagesResponse response = downloadUpdateStatus(client);
        Integer version = response.getVersion();
        downloadTilesUpdate(client, version);
        DataHandler.getInstance().notifyMapUpdated();
        Log.i(LOG_TAG, "Map update complete");
    }

    private MapImagesResponse downloadUpdateStatus(Server.Client client) throws TException {
        Log.i(LOG_TAG, "Downloading maps to update");
        MapImagesRequest request = new MapImagesRequest();
        Integer version = DataHandler.getInstance().getMapVersion();
        if (version != null) {
            request.setAcquiredVersion(version);
        }
        MapImagesResponse response = client.getMapImages(request);
        Log.i(LOG_TAG, "Maps to update obtained");
        return response;
    }

    private void downloadTilesUpdate(Server.Client client, Integer version) throws TException, IOException {
        Log.i(LOG_TAG, "Downloading map tiles addresses");
        MapImageTilesResponse floor1Response = null;
        MapImageTilesResponse floor2Response = null;
        MapImageTilesRequest requestFloor1 = new MapImageTilesRequest(Consts.FLOOR1);
        floor1Response = client.getMapImageTiles(requestFloor1);
        MapImageTilesRequest requestFloor2 = new MapImageTilesRequest(Consts.FLOOR2);
        floor2Response = client.getMapImageTiles(requestFloor2);
        Log.i(LOG_TAG, "Map tiles addresses downloaded");
        FloorMap floor1 = translateFromThrift(floor1Response);
        FloorMap floor2 = translateFromThrift(floor2Response);
        DataHandler.getInstance().setMaps(version, floor1, floor2, 0);
    }

    private FloorMap translateFromThrift(MapImageTilesResponse thriftResponse) {
        if (thriftResponse == null) {
            return  null;
        }
        Size thriftSize = thriftResponse.getOriginalSize();
        Resolution originalSize = new Resolution(thriftSize.getWidth(), thriftSize.getHeight());
        List<ImageTiles> imageTilesThrift = thriftResponse.getZoomLevels();
        ArrayList<MapTiles> mapTiles = new ArrayList<>();

        for (ImageTiles tile : imageTilesThrift) {
            Resolution scaledSize = new Resolution(tile.getScaledSize().getWidth(), tile.getScaledSize().getHeight());
            Resolution tileSize = new Resolution(tile.getTileSize().getWidth(), tile.getTileSize().getHeight());
            List<List<String>> toCopy = tile.getTilesUrls();
            MapTiles toAdd = new MapTiles(scaledSize, tileSize, copyThriftList(tile.getTilesUrls()));
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
