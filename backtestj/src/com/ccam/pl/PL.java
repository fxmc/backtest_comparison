package com.ccam.pl;

import com.ccam.utils.MyCollections;

import java.util.Date;

public class PL {

    public Date[] dts;
    public double[] pl;
    public double[] cpl;

    protected PL() {}

    public PL(Date[] dts, double[] pl) {
        this.dts = dts;
        this.pl = pl;
        this.cpl = MyCollections.cumsum(this.pl);
    }

    public PL(Date[] dts, double[] cash, double[] posn, double[] px) {
        this.dts = dts;
        this.cpl = MyCollections.add(cash, MyCollections.mmul(posn,px));
        this.pl = MyCollections.diff(this.cpl);
        this.pl[0] = this.cpl[0];
    }

}
