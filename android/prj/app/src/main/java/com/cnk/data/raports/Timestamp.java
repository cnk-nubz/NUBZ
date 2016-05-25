package com.cnk.data.raports;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class Timestamp implements Serializable {
    private int hour;
    private int minute;
    private int second;

    public Timestamp() {
    }

    public Timestamp(Date fromDate) {
        Calendar calendar = GregorianCalendar.getInstance();
        calendar.setTime(fromDate);
        this.hour = calendar.get(Calendar.HOUR_OF_DAY);
        this.minute = calendar.get(Calendar.MINUTE);
        this.second = calendar.get(Calendar.SECOND);
    }

    public int getHour() {
        return hour;
    }

    public int getMinute() {
        return minute;
    }

    public int getSecond() {
        return second;
    }
}
