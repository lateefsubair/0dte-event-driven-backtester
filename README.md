# Event-Driven Backtesting Engine

This project is a C++20 event-driven backtesting engine for intraday market data.

I built it as the next step after working on the market-data side of quantitative finance in C++. The focus here is not on finding an optimized trading strategy. The focus is the infrastructure underneath a backtest: processing market events chronologically, generating signals, creating orders, simulating execution, updating a portfolio and measuring performance without accidentally using information from the future.

The current validation run starts with 1,000,000 SPY trades, aggregates them into 5-minute bars and runs those bars through the complete backtesting pipeline.

## What the Engine Does

At a high level:

```text
Raw SPY Trades
      |
      v
MarketTradeLoader
      |
      v
1,000,000 MarketTrade Objects
      |
      v
MarketBarAggregator
      |
      v
78 Five-Minute OHLCV Bars
      |
      v
MarketEvent
      |
      v
Strategy
      |
      v
SignalEvent
      |
      v
Portfolio
      |
      v
OrderEvent
      |
      v
ExecutionHandler
      |
      v
FillEvent
      |
      v
Portfolio / Trade
      |
      v
PerformanceAnalyzer
```

The current implementation includes:

- raw SPY trade ingestion
- chronological-order validation
- 5-minute OHLCV aggregation
- volume-conservation checks
- event-driven market processing
- strategy-generated signals
- portfolio-generated orders
- next-bar execution
- commissions and adverse slippage
- pre-trade cash validation
- mark-to-market portfolio accounting
- completed-trade tracking
- equity-curve construction
- portfolio and trade-level performance statistics
- comparison with a SPY price buy-and-hold benchmark

## Why I Built It

My first C++ project concentrated mainly on market-data infrastructure and performance.

This project moves one layer further.

Once market data has been parsed and aggregated, there is still a lot between a research idea and a credible backtest. A strategy has to observe information, generate a signal, convert that signal into an order, execute it at a price that could actually have been available, update the portfolio and then measure the result.

I wanted to build those pieces separately instead of putting the entire backtest inside one loop.

That is why I used an event-driven design.

I also wanted to understand some of the backtesting problems that can be hidden by otherwise reasonable-looking results.

One of those problems actually appeared while I was building this project. The first version of the event loop ran successfully, but the timing underneath it was wrong. Finding and fixing that became one of the most useful parts of the project.

## Event-Driven Architecture

The engine has a common base event:

```cpp
class Event {
public:
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
};
```

The main event types are:

```text
MarketEvent
SignalEvent
OrderEvent
FillEvent
```

A `MarketEvent` represents market information.

A `SignalEvent` represents what the strategy wants to do.

An `OrderEvent` represents an instruction to trade.

A `FillEvent` represents what was actually executed.

This separation is important because a signal is not an order, and an order is not a fill.

Only fills change portfolio cash and holdings.

The engine uses:

```cpp
std::shared_ptr<Event>
```

so different event types can move through the same event queue.

This was also useful from the C++ side because the queue originally handled only `MarketEvent` objects. Generalizing it required inheritance, runtime polymorphism and smart pointers rather than building a separate queue for every event type.

## Market-Data Pipeline

The current validation dataset contains:

```text
1,000,000 SPY trades
```

Each raw trade contains:

```text
timestamp
symbol
price
size
side
sequence
```

The raw timestamps are represented as nanosecond Unix timestamps.

Before aggregation, I check whether the trade stream is chronologically ordered.

For the current dataset:

```text
Loaded Market Trades:  1,000,000
Out-of-Order Trades:           0
```

The trades are then aggregated into 5-minute OHLCV bars.

The 5-minute interval is:

```text
300 seconds
= 300,000,000,000 nanoseconds
```

For each interval, the aggregator constructs:

```text
Open
High
Low
Close
Volume
```

The final dataset contains:

```text
78 five-minute bars
```

Each `MarketBar` is then converted into a `MarketEvent` and passed to the backtesting engine.

## Aggregation Validation

I kept several simple checks around the aggregation stage.

One of the most important is volume conservation.

For the current run:

```text
Total Trade Volume: 64,392,783
Total Bar Volume:   64,392,783
```

The raw trades and aggregated bars therefore contain exactly the same total volume.

The first aggregated bar is:

```text
Timestamp: 2023-01-03 09:00:00
Symbol:    SPY
Open:      385.94
High:      386.16
Low:       384.99
Close:     385.11
Volume:    819109
```

The last is:

```text
Timestamp: 2023-01-03 15:25:00
Symbol:    SPY
Open:      396.42
High:      397.79
Low:       396.29
Close:     397.75
Volume:    822062
```

For me, getting zero out-of-order trades, 78 bars and exact volume conservation was an important correctness check before trusting anything produced by the backtester.

## A Timing Problem I Had to Fix

This was probably the most useful part of the project.

The first implementation put multiple market events into the event queue. Signals and orders generated from those events were then appended to the same FIFO queue.

There was a subtle problem.

A later `MarketEvent` could update the latest market price before an order generated from an earlier event reached the execution handler.

The program ran normally and the resulting prices looked reasonable, but an earlier order could effectively execute using information from the future.

I reduced the problem to a smaller test and confirmed the source of the error.

The solution was to move control of market time into `BacktestEngine`.

The engine now processes one market event and its downstream consequences before advancing the market clock:

```text
Market Event t
      |
      v
Process downstream events
      |
      v
Finish t
      |
      v
Advance to Market Event t+1
```

That removed the future-price problem.

But it raised another important question.

If the strategy observes the completed close of bar `t`, should an order based on that information also be allowed to execute at that same close?

For this project, I decided it should not.

## Next-Bar Execution

Orders generated from completed bar `t` are stored as pending orders.

They execute when bar `t+1` arrives:

```text
Bar t completes
      |
      v
Strategy observes Bar t
      |
      v
Signal
      |
      v
Order
      |
      v
PENDING
      |
      v
Bar t+1 arrives
      |
      v
Execute at Bar t+1 Open
```

One example from the validation run was:

```text
09:05
LONG signal generated
BUY order becomes pending

09:10
Next Bar Open: 385.82
BUY order executed
```

The resulting fill was:

```text
Market Price: 385.82
Fill Price:   385.859
Commission:   $1.00
```

This makes the information timing explicit. The strategy cannot observe a completed bar and then pretend it traded before that information became available.

## Slippage and Transaction Costs

The execution handler currently assumes:

```text
Commission:       $1.00 per fill
Adverse Slippage: 1 basis point
```

One basis point is:

```text
0.01%
= 0.0001
```

For a BUY:

```text
Fill Price = Market Price × (1 + 0.0001)
```

For a SELL:

```text
Fill Price = Market Price × (1 - 0.0001)
```

So simulated execution always moves slightly against the strategy.

I deliberately kept the execution model simple, but I did not want the backtest to assume free trading at perfectly favorable prices.

Before a BUY is executed, the portfolio also checks whether sufficient cash is available to fund the transaction.

## Strategy

The strategy used for the current validation run is deliberately simple.

For each 5-minute bar:

```text
Bar Return = (Close - Open) / Open
```

The momentum threshold is:

```text
0.001
```

or 0.10%.

If the bar return exceeds the threshold, the strategy generates a `LONG` signal.

Otherwise, it generates an `EXIT` signal.

I did not optimize the threshold on this dataset.

Doing that on one trading session would introduce an obvious overfitting problem and would also miss the purpose of the project.

The strategy is mainly there to exercise the full pipeline:

```text
market data
→ signal
→ order
→ execution
→ portfolio
→ trade
→ performance
```

The backtesting architecture is the project. The momentum rule is just one strategy running through it.

## Portfolio Accounting

The portfolio starts with:

```text
$100,000
```

The current implementation uses fixed 100-share positions.

The portfolio tracks cash, the current position, entry information and completed trades.

Market value is:

```text
Position × Current Market Price
```

Total equity is:

```text
Cash + Market Value
```

The portfolio is marked to market as new market events arrive.

Again, only a `FillEvent` changes the portfolio.

When a BUY fill occurs, cash decreases and the position increases.

When the position is sold, cash increases and a completed `Trade` is created.

A completed trade records:

```text
symbol
entry timestamp
exit timestamp
entry price
exit price
quantity
entry commission
exit commission
```

Gross P&L, net P&L and trade return are derived from those values.

## Execution Timestamps

Next-bar execution exposed another timing issue during development.

Originally, the fill inherited the timestamp of the order.

That was reasonable when the order and execution occurred together, but it became incorrect once orders were delayed until the next bar.

The execution handler now receives the actual execution timestamp separately.

So:

```text
Signal / Order: 09:05
Actual Fill:    09:10
```

is stored as a 09:10 trade entry.

This does not change the economics of the trade, but it makes the trade record consistent with what the simulator actually did.

## End-of-Backtest Handling

The final market bar requires special treatment.

If an order is still pending when the data ends, there is no next bar on which to execute it, so the pending order is discarded.

If the portfolio already has an open position, the engine closes that position using the final available market information.

The final equity observation is then updated rather than creating an additional artificial period.

This keeps the equity curve aligned with the 78 market bars.

## Performance Analytics

The engine calculates both portfolio-level and trade-level statistics.

Portfolio statistics include:

```text
Average Return
Volatility
Sharpe Ratio
Maximum Drawdown
Total Return
Positive Period Rate
```

The Sharpe ratio uses the 5-minute equity returns and a zero risk-free rate.

It is not annualized.

Trade statistics include:

```text
Trade Count
Trade Win Rate
Average Trade P&L
Profit Factor
Total Net P&L
Average Winning Trade
Average Losing Trade
```

I keep `Positive Period Rate` and `Trade Win Rate` separate.

Positive Period Rate measures the fraction of equity-return periods that were positive.

Trade Win Rate measures the fraction of completed trades with positive net P&L.

They are not the same statistic.

## Validation Results

The final validation run produced:

```text
Raw SPY Trades:            1,000,000
Out-of-Order Trades:               0
5-Minute Bars:                     78
Total Trade Volume:        64,392,783
Total Bar Volume:          64,392,783
Completed Trades:                  18
```

Performance:

```text
Initial Capital:          $100,000

Trade Win Rate:              50.0%
Total Net P&L:              $378.39
Average Trade P&L:           $21.02
Profit Factor:                 2.265

Total Return:                 0.378%
Maximum Drawdown:             0.236%
Sharpe Ratio:                 0.103
```

These are results from the simple momentum strategy on the current validation session.

I do not interpret them as evidence that the strategy itself is profitable.

The purpose of the run is to validate the engine using real market data through the complete pipeline.

## Benchmark Comparison

I also calculate a simple SPY price buy-and-hold return over the same interval.

```text
Strategy Return:          0.378%
SPY Price Return:         3.282%
Difference:              -2.904%
```

There is an important qualification.

The strategy starts with $100,000 but uses a fixed 100-share position. It therefore does not maintain the same exposure as a fully invested SPY portfolio.

The SPY calculation is a price-return benchmark, not an exposure-matched portfolio benchmark.

I use it as context rather than interpreting the difference as alpha.

## Small Regression Dataset

The million-trade dataset was not the first input used by the engine.

During development, I started with three hard-coded `MarketEvent` objects.

I then created a small CSV file containing the same three bars and replaced the hard-coded feed with `MarketDataLoader`.

Because the values were identical, I could check whether the CSV-driven version reproduced the behavior of the original hard-coded version.

That small dataset is retained as:

```text
data/spy_bars_test.csv
```

This was useful because it let me change the data-input layer without simultaneously changing the expected backtest behavior.

The real-data pipeline came afterward:

```text
Hard-Coded MarketEvents
          |
          v
3-Bar CSV Validation
          |
          v
1,000,000 Raw SPY Trades
          |
          v
78 Aggregated MarketEvents
```

## Building the Project

The project requires:

- C++20
- CMake
- a C++20-compatible compiler
- Linux or WSL2

Configure:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/project2_event_driven_backtester
```

The raw-trade loader expects:

```text
timestamp,symbol,price,size,side,sequence
```

## Repository Structure

```text
.
├── include/
│   ├── BacktestEngine.h
│   ├── Event.h
│   ├── EventQueue.h
│   ├── ExecutionHandler.h
│   ├── FillEvent.h
│   ├── MarketBar.h
│   ├── MarketBarAggregator.h
│   ├── MarketDataLoader.h
│   ├── MarketEvent.h
│   ├── MarketTrade.h
│   ├── MarketTradeLoader.h
│   ├── OrderEvent.h
│   ├── PerformanceAnalyzer.h
│   ├── Portfolio.h
│   ├── SignalEvent.h
│   ├── Strategy.h
│   ├── TimestampUtils.h
│   └── Trade.h
│
├── src/
│   ├── BacktestEngine.cpp
│   ├── EventQueue.cpp
│   ├── ExecutionHandler.cpp
│   ├── FillEvent.cpp
│   ├── main.cpp
│   ├── MarketBar.cpp
│   ├── MarketBarAggregator.cpp
│   ├── MarketDataLoader.cpp
│   ├── MarketEvent.cpp
│   ├── MarketTrade.cpp
│   ├── MarketTradeLoader.cpp
│   ├── OrderEvent.cpp
│   ├── PerformanceAnalyzer.cpp
│   ├── Portfolio.cpp
│   ├── SignalEvent.cpp
│   ├── Strategy.cpp
│   ├── TimestampUtils.cpp
│   └── Trade.cpp
│
├── data/
│   └── spy_bars_test.csv
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Data

The large raw market-data file is not included in this repository.

The current validation run uses the SPY trade dataset prepared during my earlier market-data work.

The repository contains the backtesting engine and the small regression dataset rather than duplicating the million-trade input file.

## What I Deliberately Did Not Do

I did not optimize the momentum strategy on this trading session.

I also did not keep adding features simply to make the project larger.

The current implementation does not attempt to support every possible combination of:

```text
multiple symbols
short positions
limit-order simulation
dynamic position sizing
multi-day portfolio state
multiple simultaneous strategies
```

Those are natural extensions.

For this project, I preferred to get chronology, execution, portfolio accounting and performance measurement right before adding more strategy complexity.

## What I Learned

The biggest lesson from this project was that a backtest can be logically wrong while still producing completely reasonable-looking output.

The original timing problem did not crash the program.

It did not produce an obviously impossible price.

It simply allowed the wrong market state to reach an earlier order.

That makes this type of error more dangerous than an ordinary programming error.

Building the engine as:

```text
MarketEvent
→ SignalEvent
→ OrderEvent
→ FillEvent
→ Portfolio
```

made it much easier to reason about when information becomes available and when a trade is actually allowed to occur.

The other important lesson was separating strategy logic from execution logic.

The strategy decides what it wants to do.

The portfolio decides whether an order is appropriate given the current position and available cash.

The execution handler determines what actually gets filled and at what price.

Those responsibilities are now separate.

## Where This Fits

This project follows my high-performance 0DTE market-data engine.

The first project concentrated on market-data ingestion, aggregation, synchronization and performance.

This project concentrates on what happens after market data reaches a strategy.

For me, the progression is:

```text
Market Data Infrastructure
          |
          v
Market Aggregation
          |
          v
Event-Driven Backtesting
          |
          v
Signals and Execution
          |
          v
Portfolio Performance
```

The strategy in this repository is intentionally simple.

For this project, I wanted to get the backtesting machinery underneath it right first.