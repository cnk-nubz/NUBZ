package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.thrift.Server;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;

import java.io.IOException;

public abstract class ServerTask extends Task {

    protected static final String LOG_TAG = "ServerTask";
    private static final String SEND_ADDRESS = "192.168.1.112";
    private static final int SEND_PORT = 9090;
    protected long delay = 1;
    protected Notificator notificator;

    public ServerTask(Notificator notificator) {
        this.notificator = notificator;
    }

    @Override
    public void run(int tries) {
        Log.i(LOG_TAG, "Starting run");
        TFramedTransport socket = openSocket(tries);
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);
        while (tries > 0 && socket != null) {
            try {
                performInSession(client);
                notificator.success();
                Log.i(LOG_TAG, "Action successful");
                socket.close();
                return;
            } catch (Exception e) {
                Log.e(LOG_TAG, "Action failed, remaining tries: " + Integer.toString(tries));
                e.printStackTrace();
                Util.waitDelay(delay * 1000);
                delay += 2;
                tries--;
            }
        }
        notificator.failure();
        if (socket != null) {
            socket.close();
        }

    }

    protected abstract void performInSession(Server.Client client) throws TException, IOException;

    private TFramedTransport openSocket(Integer tries) {
        TFramedTransport socket = new TFramedTransport(new TSocket(SEND_ADDRESS, SEND_PORT));
        Log.i(LOG_TAG, "Opening socket for address " + SEND_ADDRESS);
        while (tries > 0) {
            try {
                socket.open();
                Log.i(LOG_TAG, "Opened socket");
                return socket;
            } catch (org.apache.thrift.transport.TTransportException transportException) {
                tries--;
                Log.e(LOG_TAG, "Socket open failed, remaining tries: " + Integer.toString(tries));
                Util.waitDelay(delay * 1000);
                delay += 2;
            }
        }
        return null;
    }

}
