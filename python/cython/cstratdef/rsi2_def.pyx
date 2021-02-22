import math

cpdef void run_strategy(list close, list ma_long, list ma_short, list rsi_n, float lower_rsi, int start_indx, list posn, list cash):
    """Cythonized function with explicit type definitions"""
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
