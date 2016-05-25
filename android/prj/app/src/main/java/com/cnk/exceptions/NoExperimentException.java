package com.cnk.exceptions;

public class NoExperimentException extends Exception {
    public NoExperimentException() {
    }

    public NoExperimentException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public NoExperimentException(Throwable throwable) {
        super(throwable);
    }

    public NoExperimentException(String detailMessage) {
        super(detailMessage);
    }
}