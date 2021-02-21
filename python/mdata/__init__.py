import pandas as pd

def loadcsv() -> pd.DataFrame:
    df = pd.read_csv('..\\..\\data\\spy.csv', parse_dates=['date'])
    return df