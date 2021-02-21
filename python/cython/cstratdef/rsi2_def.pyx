import pandas as pd
import math

def first_valid_index(*args):
    ix_list = []
    for series in args:
        ix_list.append(series.first_valid_index())
    return max(ix_list)


cpdef void run_strategy(list close, list ma_long, list ma_short, list rsi_n, float lower_rsi, int start_indx, list posn, list cash):

    cdef bint long_posn = False
    cdef int idx = 0
    cdef double shares = 0.0

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


def run_strategy_df(data: pd.DataFrame, lower_rsi: float, start_indx: int, posn: list, cash: list):
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

