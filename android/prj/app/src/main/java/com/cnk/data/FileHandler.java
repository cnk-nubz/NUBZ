package com.cnk.data;

import android.os.Environment;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class FileHandler {

    private static final String DATA_PATH = Environment.getExternalStorageDirectory() + "/nubz/";
    private static final String RAPORT_DIRECTORY = "raports/";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";
    private static final String LOG_TAG = "FILE_HANDLER";

    private static FileHandler instance;

    public static FileHandler getInstance() {
        if (instance == null) {
            instance = new FileHandler();
        }
        return instance;
    }

    private FileHandler() {}

    public String saveRaportToFile(Raport raport) {
        File dir = new File(DATA_PATH + RAPORT_DIRECTORY);
        dir.mkdirs();
        File tmpFile = new File(dir, TMP);
        try {
            FileOutputStream out = new FileOutputStream(tmpFile);
            ObjectOutputStream objectOut = new ObjectOutputStream(out);
            objectOut.writeObject(raport);
            objectOut.close();
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException();
        }
        renameFile(tmpFile, RAPORT_FILE_PREFIX + raport.getId().toString());
        return RAPORT_FILE_PREFIX + raport.getId().toString();
    }

    public Raport getRaport(String filename) {
        FileInputStream in = null;
        ObjectInputStream objectIn = null;
        File dir = new File(DATA_PATH + RAPORT_DIRECTORY);
        Raport curRaport;
        try {
            in = new FileInputStream(dir.toString() + "/" + filename);
            objectIn = new ObjectInputStream(in);
            curRaport = (Raport) objectIn.readObject();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            closeInputStream(in, objectIn);
        }
        return curRaport;
    }

    private void closeInputStream(InputStream in, ObjectInputStream objectIn) {
        try {
            if (objectIn != null) {
                objectIn.close();
            }
            if (in != null) {
                in.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void renameFile(File from, String to) {
        File dir = from.getParentFile();
        File newFile = new File(dir, to);
        if (newFile.exists()) {
            newFile.delete();
        }
        from.renameTo(newFile);
    }


}
