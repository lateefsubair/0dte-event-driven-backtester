#include "MarketBarAggregator.h"
#include "TimestampUtils.h"


std::vector<MarketBar>
MarketBarAggregator::aggregateFiveMinuteBars(
    const std::vector<MarketTrade>& trades
) {
    std::vector<MarketBar> bars;

    // --------------------------------------------------
    // If there are no trades, there is nothing to
    // aggregate.
    // --------------------------------------------------

    if (trades.empty()) {
        return bars;
    }

    // --------------------------------------------------
    // Define the length of a 5-minute interval
    // in nanoseconds.
    //
    // 1 second  = 1,000,000,000 nanoseconds
    // 5 minutes = 300 seconds
    // --------------------------------------------------

    const long long nanosecondsPerSecond =
        1000000000LL;

    const long long secondsPerFiveMinutes =
        5 * 60;

    const long long fiveMinutesInNanoseconds =
        secondsPerFiveMinutes *
        nanosecondsPerSecond;


    // --------------------------------------------------
    // Determine the 5-minute bucket containing the
    // first trade.
    // --------------------------------------------------

    long long currentBucket =
        (trades[0].getTimestamp() /
         fiveMinutesInNanoseconds)
        * fiveMinutesInNanoseconds;


    // --------------------------------------------------
    // Initialize the first bar using the first trade.
    // --------------------------------------------------

    std::string currentSymbol =
        trades[0].getSymbol();

    double open =
        trades[0].getPrice();

    double high =
        trades[0].getPrice();

    double low =
        trades[0].getPrice();

    double close =
        trades[0].getPrice();

    long long volume =
        trades[0].getSize();


    // --------------------------------------------------
    // Process all remaining trades.
    // --------------------------------------------------

    for (std::size_t i = 1;
         i < trades.size();
         ++i) {

        const MarketTrade& trade =
            trades[i];


        // Determine which 5-minute bucket contains
        // the current trade.

        long long tradeBucket =
            (trade.getTimestamp() /
             fiveMinutesInNanoseconds)
            * fiveMinutesInNanoseconds;


        // --------------------------------------------------
        // SAME BUCKET
        // --------------------------------------------------

        if (tradeBucket == currentBucket) {

            if (trade.getPrice() > high) {
                high = trade.getPrice();
            }

            if (trade.getPrice() < low) {
                low = trade.getPrice();
            }

            close =
                trade.getPrice();

            volume +=
                trade.getSize();
        }


        // --------------------------------------------------
        // NEW BUCKET
        // --------------------------------------------------

        else {

            // Convert the numeric nanosecond timestamp
            // into a readable UTC date/time string.

            std::string readableTimestamp =
                TimestampUtils::nanosecondsToDateTime(
                    currentBucket
                );


            // Save the completed 5-minute bar.

            bars.emplace_back(
                readableTimestamp,
                currentSymbol,
                open,
                high,
                low,
                close,
                volume
            );


            // Start the next bar.

            currentBucket =
                tradeBucket;

            currentSymbol =
                trade.getSymbol();

            open =
                trade.getPrice();

            high =
                trade.getPrice();

            low =
                trade.getPrice();

            close =
                trade.getPrice();

            volume =
                trade.getSize();
        }
    }


    // --------------------------------------------------
    // Save the final bar.
    // --------------------------------------------------

    std::string readableTimestamp =
        TimestampUtils::nanosecondsToDateTime(
            currentBucket
        );

    bars.emplace_back(
        readableTimestamp,
        currentSymbol,
        open,
        high,
        low,
        close,
        volume
    );


    return bars;
}