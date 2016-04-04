package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.NetworkHandler;
import com.cnk.communication.thrift.Server;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;

import java.io.IOException;
import java.util.Date;

public abstract class ServerTask extends Task {

    protected static final String LOG_TAG = "ServerTask";
    private static final String SEND_ADDRESS = "192.168.0.18";
    private static final int SEND_PORT = 9090;
    private NetworkHandler.FinishAction failure;
    private NetworkHandler.FinishAction success;
    private Task.TimeoutAction timeoutAction;
    protected long delay = 1;
    protected Long beginTime;

    @Override
    public Long getBeginTime() {
        return beginTime;
    }

    public ServerTask(NetworkHandler.FinishAction failure,
                      NetworkHandler.FinishAction success,
                      Task.TimeoutAction timeoutAction) {
        this.failure = failure;
        this.success = success;
        this.timeoutAction = timeoutAction;
    }

    public Task.TimeoutAction getTimeoutAction() {
        return timeoutAction;
    }

    @Override
    public void run(int tries) {
        Log.i(LOG_TAG, "Starting run");
        if (beginTime == null) {
            beginTime = new Date().getTime();
        }

        TFramedTransport socket = openSocket(tries);
        Log.i(LOG_TAG,
              "BEGIN TIME: " + beginTime.toString() + " TIMEOUT: " + Long.toString(getTimeout()));
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);
        while (tries > 0 && socket != null && Util.checkIfBeforeTimeout(beginTime, getTimeout())) {
            try {
                performInSession(client);
                Log.i(LOG_TAG, "Action successful");
                socket.close();
                success.perform(this);
                return;
            } catch (Exception e) {
                Log.e(LOG_TAG, "Action failed, remaining tries: " + Integer.toString(tries));
                e.printStackTrace();
                Util.waitDelay(delay);
                delay += 2;
                tries--;
            }
        }
        if (socket != null) {
            socket.close();
        }
        failure.perform(this);
    }

    protected abstract void performInSession(Server.Client client) throws TException, IOException;

    private TFramedTransport openSocket(int tries) {
        TFramedTransport socket = new TFramedTransport(new TSocket(SEND_ADDRESS, SEND_PORT));
        Log.i(LOG_TAG, "Opening socket for address " + SEND_ADDRESS);
        while (tries > 0 && Util.checkIfBeforeTimeout(beginTime, getTimeout())) {
            try {
                socket.open();
                Log.i(LOG_TAG, "Opened socket");
                return socket;
            } catch (org.apache.thrift.transport.TTransportException transportException) {
                tries--;
                Log.e(LOG_TAG, transportException.toString());
                Log.e(LOG_TAG, "Socket open failed, remaining tries: " + Integer.toString(tries));
                Util.waitDelay(delay);
                delay += 2;
            }
        }
        return null;
    }

}
