package com.ccam.trading;

import java.util.Date;


public class Trade {
    Date tradeDate;
    double tradePrice;
    int posnId;
    TradeDirection tradeDiretion;

    public Trade(Date td, double tp, int pi, TradeDirection tdn) {
        tradeDate = td;
        tradePrice = tp;
        posnId = pi;
        tradeDiretion = tdn;
    }
}
