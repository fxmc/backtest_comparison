package com.ccam.data;

import com.ccam.dbg.Dbg;
import com.ccam.utils.Utility;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class OHLC {
    public Date[] dts = null;
    public double[] open = null;
    public double[] high = null;
    public double[] low = null;
    public double[] close = null;
    public long[] volume = null;
    public double[] adjclose = null;

    public int len() {
        if (dts == null) return 0;
        return dts.length;
    }

    private void addEntry(String[] entries, SimpleDateFormat sdf, List<Date> dts, List<Double> open, List<Double> high,
                          List<Double> low, List<Double> close, List<Long> volume, List<Double> adjclose)
            throws NumberFormatException, ParseException
    {
        dts.add(sdf.parse(entries[0]));
        open.add(Double.parseDouble(entries[1]));
        high.add(Double.parseDouble(entries[2]));
        low.add(Double.parseDouble(entries[3]));
        close.add(Double.parseDouble(entries[4]));
        volume.add(Long.parseLong(entries[5]));
        adjclose.add(Double.parseDouble(entries[6]));
    }

    private int getApproxSize(String fileName) throws IOException {
        File f = new File(fileName);
        if (!f.exists()) {
            throw( new IOException("Input file not found!"));
        }
        return (int)(f.length()/50);
    }

    private BufferedReader getFile(String fileName) throws IOException{
        File f = new File(fileName);
        if (!f.exists()) {
            throw( new IOException("Input file not found!"));
        }
        return new BufferedReader(new FileReader(fileName), 1_000_000);
    }

    private void loadCSV(BufferedReader in, List<Date> dts, List<Double> open, List<Double> high, List<Double> low,
                         List<Double> close, List<Long> volume, List<Double> adjclose)
            throws IOException, NumberFormatException, ParseException
    {
        SimpleDateFormat sdf = new SimpleDateFormat("M/d/yyyy");
        in.readLine();

        try {
            String s = in.readLine();
            String[] t;
            while (s != null) {
                t = s.split(",");
                addEntry(t, sdf, dts, open, high, low, close, volume, adjclose);
                s = in.readLine();
            }
        }
        catch (IOException io) {
            Dbg.print(io);
            Utility.cleanup(in);
            throw(io);
        }
        finally {
            Utility.cleanup(in);
        }

    }

    public void readCSV(String fileName) throws IOException, NumberFormatException, ParseException {
        int sz = getApproxSize(fileName);

        List<Date> dts_o = new ArrayList<>(sz);
        List<Double> open_o = new ArrayList<>(sz);
        List<Double> high_o = new ArrayList<>(sz);
        List<Double> low_o = new ArrayList<>(sz);
        List<Double> close_o = new ArrayList<>(sz);
        List<Long> volume_o = new ArrayList<>(sz);
        List<Double> adjclose_o = new ArrayList<>(sz);

        BufferedReader in = getFile(fileName);

        loadCSV(in, dts_o, open_o, high_o, low_o, close_o, volume_o, adjclose_o);

        dts = dts_o.toArray(new Date[0]);
        open = Utility.toPrimitiveDoubleArray(open_o);
        high = Utility.toPrimitiveDoubleArray(high_o);
        low = Utility.toPrimitiveDoubleArray(low_o);
        close = Utility.toPrimitiveDoubleArray(close_o);
        volume = Utility.toPrimitiveLongArray(volume_o);
        adjclose = Utility.toPrimitiveDoubleArray(adjclose_o);
    }
}
