package com.cnk.exceptions;

public class InternalDatabaseError extends Error {
    public InternalDatabaseError() {
    }

    public InternalDatabaseError(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public InternalDatabaseError(Throwable throwable) {
        super(throwable);
    }

    public InternalDatabaseError(String detailMessage) {
        super(detailMessage);
    }
}
