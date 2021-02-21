import pandas as pd
import math
import mdata
import ta.indicators as indi
import time


def run_strategy(close: list, ma_long: list, ma_short: list, rsi_n: list, lower_rsi: float, start_indx: int,
                 posn: list, cash: list):
    long_posn = False

    for idx in range(start_indx, len(close)):
        posn[idx] = posn[idx-1]
        cash[idx] = cash[idx-1]

        if long_posn:
            if close[idx] > ma_short[idx]:
                long_posn = False
                shares = posn[idx]
                posn[idx] = 0
                cash[idx] = cash[idx] + shares * close[idx]

        if not long_posn:
            if (close[idx] > ma_long[idx]) and (rsi_n[idx] < lower_rsi):
                long_posn = True
                shares = math.floor(cash[idx] / close[idx])
                posn[idx] = shares
                cash[idx] = cash[idx] - shares * close[idx]

    return cash, posn


if __name__ == "__main__":
    ticker = 'SPY'
    PX = mdata.loadcsv()
    RSI_PERIOD = 2
    MA_LONG_PERIOD = 200
    MA_SHORT_PERIOD = 5
    RSI_N = indi.rsi(PX['close'], RSI_PERIOD)
    MA_LONG = indi.sma(PX['close'], MA_LONG_PERIOD)
    MA_SHORT = indi.sma(PX['close'], MA_SHORT_PERIOD)
    STARTING_CAPITAL = 10_000.0

    CLOSE = PX['close'].tolist()
    MA_LONG = MA_LONG.tolist()
    MA_SHORT = MA_SHORT.tolist()
    RSI_N = RSI_N.tolist()
    SIX = 199
    POSN = len(CLOSE) * [0]
    CASH = len(CLOSE) * [STARTING_CAPITAL]

    DATA = pd.DataFrame([CLOSE, MA_LONG, MA_SHORT, RSI_N]).transpose()
    DATA.columns = ['close', 'ma_long', 'ma_short', 'rsi']

    ST_TM = time.time()
    for IX in range(0, 1000):
        run_strategy(CLOSE, MA_LONG, MA_SHORT, RSI_N, 20.0, SIX, POSN, CASH) # Time in seconds for run_strategy: 0.003114142894744873
    ED_TM = time.time()
    TM = (ED_TM - ST_TM) / 1000
    print("Time in seconds for run_strategy: {}".format(str(TM)))
