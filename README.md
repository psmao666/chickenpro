# chickenpro

Welcome to ChickenPro (chickenproject 2.0)!

First thing you need to do is to run an virtual environment with Python

```
python -m venv env
```

Then, run dataCollector.py to obtain historical data of one specific instrument

You need to modify the code lel.

The backtester was coded in strategy_mean_reverse.cpp, so just simply compile it with c++17 and run it

it should generate a report named "report.txt", which reflects the backtest log of the last four years.

Feel free to modify anything :)

You can also play around with the plotter.py to plot your result (but you need to modify the output of the backtester)
