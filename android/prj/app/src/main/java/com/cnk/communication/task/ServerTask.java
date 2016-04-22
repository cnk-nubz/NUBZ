package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.NetworkHandler;
import com.cnk.communication.thrift.Server;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;

import java.io.IOException;

public abstract class ServerTask extends Task {

    public enum FailureReason {
        NOWIFI("no wifi"),
        SOCKET_OPEN_FAILED("socket opening failed"),
        ACTION_FAILED("action on opened socket failed");

        private String description;

        FailureReason(String description) {
            this.description = description;
        }

        @Override
        public String toString() {
            return description;
        }
    }

    protected static final String LOG_TAG = "ServerTask";
    private static final String SEND_ADDRESS = "zpp.dns1.us";
    private static final int SEND_PORT = 9090;
    private NetworkHandler.SuccessAction success;
    private NetworkHandler.FailureAction failure;

    public ServerTask(NetworkHandler.SuccessAction success, NetworkHandler.FailureAction failure) {
        this.success = success;
        this.failure = failure;
    }

    @Override
    public void run() {
        Log.i(LOG_TAG, "Starting run: " + getTaskName());

        if (!NetworkHandler.getInstance().isConnectedToWifi()) {
            notifyFailure(FailureReason.NOWIFI);
            return;
        }

        TFramedTransport socket = openSocket();
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);
        if (socket == null) {
            notifyFailure(FailureReason.SOCKET_OPEN_FAILED);
            return;
        }

        try {
            performInSession(client);
        } catch (Exception e) {
            e.printStackTrace();
            notifyFailure(FailureReason.ACTION_FAILED);
            return;
        } finally {
            socket.close();
        }

        notifySuccess();
    }

    protected abstract void performInSession(Server.Client client) throws TException, IOException;

    private TFramedTransport openSocket() {
        TFramedTransport socket = new TFramedTransport(new TSocket(SEND_ADDRESS, SEND_PORT));
        Log.i(LOG_TAG, "Opening socket for address " + SEND_ADDRESS);
        try {
            socket.open();
            Log.i(LOG_TAG, "Opened socket");
        } catch (org.apache.thrift.transport.TTransportException transportException) {
            Log.e(LOG_TAG, transportException.toString());
            socket = null;
        }
        return socket;
    }

    private void notifyFailure(FailureReason reason) {
        Log.e(LOG_TAG, "Task failed: " + getTaskName() + ", reason: " + reason);
        if (failure != null) {
            failure.perform(this, reason);
        }
    }

    private void notifySuccess() {
        Log.i(LOG_TAG, "Task succeeded: " + getTaskName());
        if (success != null) {
            success.perform(this);
        }
    }

}
