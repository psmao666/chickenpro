import requests
import pandas as pd
from datetime import datetime

def get_price(pair_name):
    # expecting a pair name in the form of "XXX-XXX"
    url = "https://www.okx.com"
    ticker = requests.get(url+'/api/v5/market/ticker?instId='+pair_name).json()
    return ticker

def get_historical_data(pair_name, after, time_interval):

    if after == 'na':
        historical = pd.DataFrame((requests.get(url+f'/api/v5/market/history-candles?instId={pair_name}&bar={time_interval}').json())['data'])
    else:
        historical = pd.DataFrame((requests.get(url+f'/api/v5/market/history-candles?instId={pair_name}&after={after}&bar={time_interval}').json())['data'])

    historical.columns = ["Date","Open","High","Low","Close","Vol","volCcy","volCcyQuote","confirm"]
    last_time = historical['Date'].iloc[-1]
    historical['Date'] = pd.to_datetime(pd.to_numeric(historical['Date']), unit='ms')
    return last_time, historical

if __name__ == "__main__":
    url = "https://www.okx.com"
    last_time = 'na'
    history = pd.DataFrame()
    pair_name = "ETH-USDT"
    time_interval = "1H"

    print("Now downloading data.....")
    for i in range(1, 400):
        print(f"downloading page {i}")
        last_time, tmp_hist = get_historical_data(pair_name, last_time, time_interval)
        history = pd.concat([history, tmp_hist])
    
    print(history)
    history.to_csv(pair_name+'.csv', index=False)