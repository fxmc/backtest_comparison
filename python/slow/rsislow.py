import datetime as dt
import pandas as pd
import math
import ta.indicators as indi
import time
import mdata

from collections import namedtuple
from enum import Enum


class TradeDirection(Enum):
    ENTRYLONG = 0
    EXITLONG = 1
    ENTRYSHORT = 2
    EXITSHORT = 3


Trade = namedtuple('Trade', ['trade_date', 'trade_price', 'posn_id', 'trade_direction'])


def first_valid_index(*args):
    ix_list = []
    for series in args:
        ix_list.append(series.first_valid_index())
    return max(ix_list)


def run_strategy(close: pd.Series,
                 ma_long: pd.Series, ma_short: pd.Series, rsi_n: pd.Series,
                 starting_capital: float, lower_rsi: float, start_indx):

    dt_range = close.index[close.index.get_loc(start_indx):]

    posn = pd.Series(0, index=close.index)
    cash = pd.Series(starting_capital, index=close.index)
    trades = []

    long_posn = False
    posn_id = 0

    for d in dt_range:
        posn[d] = posn.shift(1)[d]
        cash[d] = cash.shift(1)[d]

        if long_posn:
            if close[d] > ma_short[d]:
                long_posn = False
                trade = Trade(trade_date=d, trade_price=close[d],
                              posn_id=posn_id, trade_direction=TradeDirection.EXITLONG)
                trades.append(trade)
                shares = posn[d]
                posn[d] = 0
                cash[d] = cash[d] + shares * close[d]
                posn_id += 1

        if not long_posn:
            if (close[d] > ma_long[d]) and (rsi_n[d]<lower_rsi):
                long_posn = True
                trade = Trade(trade_date=d, trade_price=close[d],
                              posn_id=posn_id, trade_direction=TradeDirection.ENTRYLONG)
                trades.append(trade)
                shares = math.floor(cash[d] / close[d])
                posn[d] = shares
                cash[d] = cash[d] - shares * close[d]

    return cash, posn, trades


if __name__ == "__main__":
    PX = mdata.loadcsv()
    RSI_PERIOD = 2
    MA_LONG_PERIOD = 200
    MA_SHORT_PERIOD = 5
    RSI_N = indi.rsi(PX['close'], RSI_PERIOD)
    MA_LONG = indi.sma(PX['close'], MA_LONG_PERIOD)
    MA_SHORT = indi.sma(PX['close'], MA_SHORT_PERIOD)
    RSI_LOWER = 20.0
    SIX = first_valid_index(PX['close'], MA_LONG, MA_SHORT, RSI_N)
    STARTING_CAPITAL = 10_000.0
    print('starting')
    ST_TM = time.time()
    for IX in range(0,10):
        [CASH1, POSN1, TRADES1] = run_strategy(PX['close'], MA_LONG, MA_SHORT, RSI_N, STARTING_CAPITAL, RSI_LOWER, SIX)
    ED_TM = time.time()
    TM = (ED_TM - ST_TM) / 10
    print("Time in seconds for run_strategy: {}".format(str(TM)))



