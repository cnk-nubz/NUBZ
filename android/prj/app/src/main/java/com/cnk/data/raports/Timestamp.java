package com.cnk.data.raports;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class Timestamp implements Serializable {
    private Integer hour;
    private Integer minute;
    private Integer second;

    public Timestamp() {
    }

    public Timestamp(Date fromDate) {
        Calendar calendar = GregorianCalendar.getInstance();
        calendar.setTime(fromDate);
        this.hour = calendar.get(Calendar.HOUR_OF_DAY);
        this.minute = calendar.get(Calendar.MINUTE);
        this.second = calendar.get(Calendar.SECOND);
    }

    public Integer getHour() {
        return hour;
    }

    public Integer getMinute() {
        return minute;
    }

    public Integer getSecond() {
        return second;
    }
}
