package com.cnk.data;

import android.util.Log;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class FileHandler {

    private static final String LOG_TAG = "FILE_HANDLER";

    private static FileHandler instance;

    public static FileHandler getInstance() {
        if (instance == null) {
            instance = new FileHandler();
        }
        return instance;
    }

    private FileHandler() {}

    public void saveInputStream(InputStream in, String filename) throws IOException {
        File file = new File(filename);
        file.getParentFile().mkdirs();
        FileOutputStream out = new FileOutputStream(file);
        IOUtils.copy(in, out);
        out.close();
        in.close();
    }

    public void saveSerializable(Serializable toSave, String filename) throws IOException {
        File file = new File(filename);
        file.getParentFile().mkdirs();
        FileOutputStream out = new FileOutputStream(file);
        ObjectOutputStream objOut = new ObjectOutputStream(out);
        objOut.writeObject(toSave);
        out.close();
        objOut.close();
    }

    public FileInputStream getFile(String filename) throws FileNotFoundException {
        File file = new File(filename);
        return new FileInputStream(file);
    }

    public void renameFile(String from, String to) throws IOException {
        if (from == null) {
            return;
        }
        File file = new File(from);
        File dir = file.getParentFile();
        File newFile = new File(dir, to);
        Log.i(LOG_TAG, "Renaming " + from + " to " + to);
        if (newFile.exists()) {
            if (newFile.isDirectory()) {
                FileUtils.deleteDirectory(newFile);
            } else {
                newFile.delete();
            }
        }
        file.renameTo(newFile);
    }
}
