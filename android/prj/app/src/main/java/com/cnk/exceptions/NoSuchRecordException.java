package com.cnk.exceptions;

public class NoSuchRecordException extends RuntimeException {
    public NoSuchRecordException() {
    }

    public NoSuchRecordException(String detailMessage) {
        super(detailMessage);
    }

    public NoSuchRecordException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public NoSuchRecordException(Throwable throwable) {
        super(throwable);
    }
}
