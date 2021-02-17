package com.ccam.dbg;

import java.text.SimpleDateFormat;
import java.util.Date;

public final class Dbg {

    private Dbg() {}

    public static final boolean DEBUG  = true;

    synchronized public static void print( Object output ) {
        if( DEBUG )
            System.out.println( Thread.currentThread().getName() + "  " + timestamp() + " DEBUG:  " + output );
    }

    synchronized public static void print( Object output, boolean special ) {
        if( DEBUG || special)
            System.out.println( Thread.currentThread().getName() + "  " + timestamp() + " DEBUG:  " + output );
    }

    synchronized public static void print( Exception e ) {
        if( DEBUG ) {
            System.out.println(Thread.currentThread().getName() + "  " +  timestamp() + " DEBUG:  Exception " + e.getClass().getName() );
            e.printStackTrace( System.out );
        }
    }

    synchronized public static void printErr( Object output ) {
        System.err.println(Thread.currentThread().getName() + "  " +  timestamp() + " DEBUG:  " + output );
    }

    synchronized private static String timestamp() {
        SimpleDateFormat f = new SimpleDateFormat( "ddMMMyy HH:mm:ss z" );
        return( f.format( new Date()));
    }


}
