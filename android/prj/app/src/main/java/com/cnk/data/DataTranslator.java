package com.cnk.data;

import java.io.FileInputStream;
import java.io.ObjectInputStream;

public class DataTranslator {

    public static Raport getRaportFromStream(FileInputStream in) throws Exception {
        ObjectInputStream objIn = new ObjectInputStream(in);
        Raport raport = (Raport) objIn.readObject();
        objIn.close();
        in.close();
        return raport;
    }
}
