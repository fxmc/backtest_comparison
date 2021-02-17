package com.ccam;

import com.ccam.data.OHLC;
import com.ccam.ta.IndicatorCalc;
import com.ccam.pl.PL;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.ParseException;
import java.util.*;


public class Main {
    private static PL rsi2_strategy(Date[] dts, double[] close, double[] ma_long, double[] ma_short, double[] rsi_n,
                                   double lower_rsi, int start_indx, double[] posn, double[] cash)
    {
        boolean long_posn = false;

        for (int idx=start_indx; idx<close.length; idx++) {
            posn[idx] = posn[idx-1];
            cash[idx] = cash[idx-1];

            if (long_posn) {
                if (close[idx] > ma_short[idx]) {
                    long_posn = false;
                    double shares = posn[idx];
                    posn[idx] = 0;
                    cash[idx] = cash[idx] + shares * close[idx];
                }
            }

            if (!long_posn) {
                if ( (close[idx] > ma_long[idx]) && (rsi_n[idx] < lower_rsi)) {
                    long_posn = true;
                    double shares =  Math.floor(cash[idx] / close[idx]);
                    posn[idx] = shares;
                    cash[idx] = cash[idx] - shares * close[idx];
                }
            }
        }

        return new PL(dts, cash, posn, close);
    }

    private static void writeResult(String fileName, OHLC ohlc, double[] ma_short, double[] ma_long, double[] rsi_n, double[] posn, double[] cash, PL pl) throws IOException{
        FileWriter csvwriter = new FileWriter(fileName);
        PrintWriter pw = new PrintWriter(csvwriter);
        pw.println("close,ma_short,ma_long,rsi_2,posn,cash,pl");
        for (int i = 0; i < ohlc.len(); i++) {
            pw.printf("%.16f,%.16f,%.16f,%.16f,%.16f,%.16f,%.16f\n", ohlc.close[i], ma_short[i], ma_long[i], rsi_n[i], posn[i], cash[i], pl.cpl[i]);
        }
        pw.flush();
        pw.close();
    }

    public static void main(String[] Args) throws IOException, NumberFormatException, ParseException {
        OHLC ohlc = new OHLC();
        String fileName = "..\\data\\spy.csv";   // Make sure to point to the rigth directory when running.
        ohlc.readCSV(fileName);

        int    rsi_period      = 2;
        int    ma_long_period  = 200;
        int    ma_short_period = 5;
        double start_capital   = 10_000.0;
        double lower_rsi       = 20.0;
        int    six             = ma_long_period-1;

        double[] rsi_n         = IndicatorCalc.rsi(ohlc.close, rsi_period);
        double[] ma_long       = IndicatorCalc.sma(ohlc.close, ma_long_period);
        double[] ma_short      = IndicatorCalc.sma(ohlc.close, ma_short_period);
        double[] posn          = new double[ohlc.close.length];
        double[] cash          = new double[ohlc.close.length];
        Arrays.fill(cash, start_capital);

        long a = System.nanoTime();
        for (int i=0; i< 100_000; i++) {
            Main.rsi2_strategy(ohlc.dts, ohlc.close, ma_long, ma_short, rsi_n, lower_rsi, six, posn, cash);
        }
        long b = System.nanoTime();
        double time = (b - a) / 100_000.0 / 1_000_000_000.0;
        System.out.println("Time taken: " + String.format("%.10f",time) + " seconds");

        PL pl = rsi2_strategy(ohlc.dts, ohlc.close, ma_long, ma_short, rsi_n, lower_rsi, six, posn, cash);
        System.out.println("Final pl : " + String.format("%.7f", pl.cpl[pl.cpl.length-1]));
        Main.writeResult("..\\data\\output.csv", ohlc, ma_short, ma_long, rsi_n, posn, cash, pl);
    }
}
