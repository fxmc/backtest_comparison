package com.ccam.utils;

public final class MyCollections {
    private MyCollections() {}

    static public double max(double[] d, boolean ignoreNaN) {
        if (d == null) {
            throw new IllegalArgumentException("The Array must not be null");
        } else if (d.length == 0) {
            throw new IllegalArgumentException("Array cannot be empty.");
        }

        int start_pos = 0;
        double max = d[start_pos];

        start_pos += 1;
        while (Double.isNaN(max) && (start_pos<d.length)) {
            max =  d[start_pos];
            start_pos += 1;
        }

        if (Double.isNaN(max) || start_pos>=d.length) {
            return max;
        }

        for (int idx=start_pos+1; idx<d.length; idx++) {
            if (Double.isNaN(d[idx])) {
                if (ignoreNaN) continue;
                return Double.NaN;
            }
            if (d[idx]>max) {
                max = d[idx];
            }
        }
        return max;
    }

    static public double max(double[] d) {
        return max(d, true);
    }

    static public double min(double[] d, boolean ignoreNaN) {
        return -max(mmul(d, -1.0), ignoreNaN);
    }

    static public double min(double[] d) {
        return min(d, true);
    }

    static public double[] mmul(double[] d, double x) {
        double[] res = new double[d.length];
        for (int idx=0; idx<d.length; idx++) {
            res[idx] = d[idx] * x;
        }
        return res;
    }

    static public double[] mmul(double x, double[] d) {
        return mmul(d, x);
    }

    static public double[] mmul(double[] a, double[] b) {
        assert a.length == b.length;
        double[] result = new double[a.length];
        for (int i = 0; i<a.length; i++) {
            result[i] = a[i] * b[i];
        }
        return result;
    }

    static public double[] mdiv(double[] d, double x) {
        double[] res = new double[d.length];
        for (int idx=0; idx<d.length; idx++) {
            res[idx] = d[idx] / x;
        }
        return res;
    }

    static public double[] mdiv(double[] a, double[] b) {
        assert a.length == b.length;
        double[] result = new double[a.length];
        for (int i = 0; i<a.length; i++) {
            result[i] = a[i] / b[i];
        }
        return result;
    }

    static public double[] cumsum(double[] d) {
        double[] result = new double[d.length];
        result[0] = d[0];
        for (int i = 1; i < d.length; i++) {
            result[i] = result[i-1] + d[i];
        }
        return result;
    }

    static public double[] add(double[] a, double[] b) {
        assert a.length == b.length;

        double[] result = new double[a.length];
        for (int i = 0; i<a.length; i++) {
            result[i] = a[i] + b[i];
        }
        return result;
    }

    static public double[] sub(double[] a, double[] b) {
        return add(a, mmul(b, -1.0));
    }

    static public double[] diff(double[] a, int n) {
        double[] result = new double[a.length];
        for (int i = 0; i<n; i++) {
            result[i] = Double.NaN;
        }
        result[0] = Double.NaN;
        for (int i = n; i<a.length; i++) {
            result[i] = a[i] - a[i-1];
        }
        return result;
    }

    static public double[] diff(double[] a) {
        return diff(a, 1);
    }
}
