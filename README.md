# Backtest Comparison

The purpose of this repo is to time a basic trading strategy implemented in different languages.

The bare metal version is in C.  The implmentation in C was especially fun to write, due to its lack of anything.  At least in languages like Java complex data types / classes exist.  Here everything is implemented from scratch.

## Instructions to build / run the various backtestsers for RSI(2) strategy

1. The data used is from Yahoo Finance and stored in SPY.csv in teh data directory.  All implementations reference this file.

2. The C implementation lies in backtestc.  It's a Visual Studio Solution.  The main entry is in main.c.  All utilities lie in backtestc/utils.  Load the Visual Studio Solution file, bactkestc.sln and build it.

3. The Java implementation lies in backtestj.  The project was setup with Intellij, hence the backtestj.iml file.  However, only standard Java was used, no external packages.

4. Regarding Python the top directory is python/ [i.e. set Pythonpath to this directory].  The three versions: slow, DataFrame optimized, and Cython are in the slow, pyoptim, cython modules respectively.  The ta module provides the indicator calculations.  The mdata module provides the csv loader for the SPY.csv file.  To build the two cython modules execute "python setup.py build_ext --inplace" and copy the resulting .pyd files into the cython directory for rsi2_cy.py to run.

The purpose was to time the various implementations on a simple naive trading strategy.  The number of observed days is ca. 7,000.  The timings are several orders of magnitude apart.  The idea was to find best compromise in Python for getting speeds of low level languages, and yet retaining the convenience of the Python eco-system.