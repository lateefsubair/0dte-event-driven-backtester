#include "TimestampUtils.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


std::string TimestampUtils::nanosecondsToDateTime(
    long long timestampNanoseconds
) {
    // --------------------------------------------------
    // Convert nanoseconds since Unix epoch into seconds.
    //
    // Example:
    //
    // 1672736400000000000 ns
    //
    // becomes:
    //
    // 1672736400 seconds
    // --------------------------------------------------

    const long long nanosecondsPerSecond =
        1000000000LL;

    std::time_t timestampSeconds =
        static_cast<std::time_t>(
            timestampNanoseconds /
            nanosecondsPerSecond
        );


    // --------------------------------------------------
    // Convert Unix time to UTC calendar time.
    //
    // We deliberately use UTC here.
    //
    // This avoids silently hard-coding an Eastern Time
    // offset and keeps the timestamp conversion correct
    // and portable.
    // --------------------------------------------------

    std::tm utcTime{};

    gmtime_r(
        &timestampSeconds,
        &utcTime
    );


    // --------------------------------------------------
    // Format:
    //
    // YYYY-MM-DD HH:MM:SS
    //
    // Example:
    //
    // 2023-01-03 14:20:00
    // --------------------------------------------------

    std::ostringstream output;

    output << std::put_time(
        &utcTime,
        "%Y-%m-%d %H:%M:%S"
    );

    return output.str();
}