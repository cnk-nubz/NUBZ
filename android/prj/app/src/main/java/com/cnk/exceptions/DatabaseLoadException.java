package com.cnk.exceptions;

import java.io.IOException;

public class DatabaseLoadException extends IOException {

    public DatabaseLoadException() {
    }

    public DatabaseLoadException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public DatabaseLoadException(Throwable throwable) {
        super(throwable);
    }

    public DatabaseLoadException(String detailMessage) {
        super(detailMessage);
    }
}
