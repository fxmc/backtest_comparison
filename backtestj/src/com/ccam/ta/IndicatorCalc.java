package com.ccam.ta;

import java.util.Arrays;

public class IndicatorCalc {

    private double _average(double[] series, int start_ix, int end_ix) {
        // Note: the ranges is start_ix <= ix < end_ix, i.e Python convention!
        double sum = 0.0;
        for (int ix = start_ix; ix<end_ix; ix++) {
            sum += series[ix];
        }
        return sum / (end_ix - start_ix);
    }

    static public double [] rsi(double[] series, int period) {
        double[] rsi_result = new double[series.length];
        Arrays.fill(rsi_result, Double.NaN);

        double up_a = 0.0;
        double dn_a = 0.0;
        for (int idx = 1; idx<=period; idx++) {
            up_a += Math.max((series[idx] - series[idx - 1]), 0.0);
            dn_a += Math.max((series[idx-1] - series[idx]), 0.0);
        }
        up_a /= period;
        dn_a /= period;
        double rs = (dn_a == 0.0) ? 1e8 : up_a / dn_a;
        rsi_result[period] = 100.0 - 100.0 / (1.0 + rs);

        for (int idx = period+1; idx<series.length; idx++) {
            double up = Math.max((series[idx] - series[idx - 1]), 0.0);
            double dn = Math.max((series[idx-1] - series[idx]), 0.0);
            up_a = ( (period-1.0) * up_a + up ) / period;
            dn_a = ( (period-1.0) * dn_a + dn ) / period;
            rs = (dn_a == 0.0) ? 1e8 : up_a / dn_a;
            rsi_result[idx] = 100.0 - 100.0 / (1.0 + rs);
        }

        return rsi_result;
    }

    static public double [] sma(double[] series, int period) {
        double[] ma_result = new double[series.length];
        Arrays.fill(ma_result, Double.NaN);

        if (period>series.length) return ma_result;

        double sum = 0.0;
        for (int idx = 0; idx<period; idx++) {
            sum += series[idx];
        }
        ma_result[period-1] = sum / period;

        for (int idx=period; idx<series.length; idx++) {
            ma_result[idx] = ma_result[idx-1]  + ( series[idx] - series[idx-period] ) / period;
        }
        return ma_result;
    }
}
