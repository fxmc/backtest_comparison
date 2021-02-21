import pandas as pd
import math
import ta.indicators as indi
import time
import mdata


def first_valid_index(*args):
    ix_list = []
    for series in args:
        ix_list.append(series.first_valid_index())
    return max(ix_list)


def run_strategy_df_tuples(data: pd.DataFrame, lower_rsi: float, start_indx: int, posn: list, cash: list):
    long_posn = False

    for row in data.itertuples():
        if row.Index < start_indx:
            continue

        posn[row.Index] = posn[row.Index - 1]
        cash[row.Index] = cash[row.Index - 1]

        if long_posn:
            if row.close > row.ma_short:
                long_posn = False
                shares = posn[row.Index]
                posn[row.Index] = 0
                cash[row.Index] = cash[row.Index] + shares * row.close

        if not long_posn:
            if (row.close > row.ma_long) and (row.rsi < lower_rsi):
                long_posn = True
                shares = math.floor(cash[row.Index] / row.close)
                posn[row.Index] = shares
                cash[row.Index] = cash[row.Index] - shares * row.close

    return cash, posn


def run_strategy_df_rows(data: pd.DataFrame, lower_rsi: float, start_indx: int, posn: list, cash: list):
    long_posn = False

    for row_ in data.iterrows():
        ix: int = row_[0]
        row = row_[1]
        if ix< start_indx:
            continue

        posn[ix] = posn[ix - 1]
        cash[ix] = cash[ix - 1]

        if long_posn:
            if row.close > row.ma_short:
                long_posn = False
                shares = posn[ix]
                posn[ix] = 0
                cash[ix] = cash[ix] + shares * row.close

        if not long_posn:
            if (row.close > row.ma_long) and (row.rsi < lower_rsi):
                long_posn = True
                shares = math.floor(cash[ix] / row.close)
                posn[ix] = shares
                cash[ix] = cash[ix] - shares * row.close

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
    for IX in range(0, 10):
        run_strategy_df_tuples(DATA, 20.0, SIX, POSN, CASH)                         # Time in seconds for run_strategy: 0.03616330647468567
    ED_TM = time.time()
    TM = (ED_TM - ST_TM) / 10
    print("Time in seconds for run_strategy: {}".format(str(TM)))

    ST_TM = time.time()
    for IX in range(0, 10):
        run_strategy_df_rows(DATA, 20.0, SIX, POSN, CASH)                            # Time in seconds for run_strategy: 1.2993709206581117
    ED_TM = time.time()
    TM = (ED_TM - ST_TM) / 10
    print("Time in seconds for run_strategy: {}".format(str(TM)))


