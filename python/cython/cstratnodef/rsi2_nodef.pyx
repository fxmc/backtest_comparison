import pandas as pd
import math

def run_strategy(close, ma_long, ma_short, rsi_n, lower_rsi, start_indx, posn, cash):
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
