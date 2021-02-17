package com.ccam.utils;

import com.ccam.dbg.Dbg;

import java.io.Closeable;
import java.io.IOException;
import java.util.List;

public final class Utility {

    protected Utility() {}

    public static void cleanup(Closeable b) {
        if (b!=null) try {
            b.close();
        } catch (IOException io) {
            Dbg.print(io);
        }
    }

    public static double[] toPrimitiveDoubleArray(List<Double> doubleArray) {
        double[] d = new double[doubleArray.size()];
        for (int idx = 0; idx<doubleArray.size(); idx++) {
            d[idx] = doubleArray.get(idx);
        }
        return d;
    }

    public static long[] toPrimitiveLongArray(List<Long> longArray) {
        long[] d = new long[longArray.size()];
        for (int idx = 0; idx<longArray.size(); idx++) {
            d[idx] = longArray.get(idx);
        }
        return d;
    }
}
