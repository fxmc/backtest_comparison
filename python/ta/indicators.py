import pandas as pd
import numpy as np


def rsi(s: pd.Series, n: int) -> pd.Series:
    '''
    Technical analysis RSI function applied to a Series, e.g. closing prices

    :param s: Series to which RSI is to be applied
    :param n: N period of RSI
    :return:  A Series with same index as s containing the RSI

    Detailed pseudo code for RSI(n) calculation:
        need
            up[i] =  max(0,px[i]-px[i-1])
            dn[i] = -min(0,px[i]-px[i-1])

        rsi[1:n] = NaN
        rsu[n] = avg(up[n],n)
        rsd[n] = avg(dn[n],n)
        rs[n]  = rsu[n] / rsd[n]

        rsu[i>n] = (up[i] + (n-1) * up[i-1]) / n
        rsd[i>n] = [dn[i] + (n-1) * dn[i-1]) / n

        rs = rsu ./ rsd
        rsi = 100 - 100 / (1 + rs)
    '''

    d1s = s.diff()
    ds = d1s[1:] # first entry will be NaN

    up , dn = ds.copy(), ds.copy()
    up[up<0] = 0
    dn[dn>0] = 0
    dn = dn.abs()

    upa = pd.concat([up.rolling(window=n,min_periods=n).mean()[:n],up[n:]])
    upa = upa.ewm(alpha = 1.0/n, adjust=False).mean()

    dna = pd.concat([dn.rolling(window=n,min_periods=n).mean()[:n],dn[n:]])
    dna = dna.ewm(alpha = 1.0/n, adjust=False).mean()

    rs  = np.divide(upa,dna)

    rsi = 100 - np.divide(100, 1 + rs)
    rsi = pd.concat([d1s[:1], rsi])
    return rsi


def bollinger_band(s : pd.Series, n : int, std_fac : float):
    '''
    Function to calculate the bollinger bands around a series.

    :param s:       The series under consideration
    :param n:       The window size for the calculation
    :param std_fac: The standard deviation window around the average
    :return:        Three series as a tuple: moving average, moving average-std_fac*std(series), moving average+std_fac*std(series)
    '''

    sma = s.rolling(window=n,min_periods=n).mean()
    std = s.rolling(window=n,min_periods=n).std()

    sma_up = sma + std_fac * std
    sma_dn = sma - std_fac * std

    return (sma, sma_up, sma_dn)


def sma(s: pd.Series, n: int) -> pd.Series:
    return s.rolling(n).mean()
