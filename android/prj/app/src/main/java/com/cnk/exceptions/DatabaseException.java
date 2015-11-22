package com.cnk.exceptions;

public class DatabaseException extends Exception {
    public DatabaseException() {
    }

    public DatabaseException(String detailMessage) {
        super(detailMessage);
    }

    public DatabaseException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public DatabaseException(Throwable throwable) {
        super(throwable);
    }
}
