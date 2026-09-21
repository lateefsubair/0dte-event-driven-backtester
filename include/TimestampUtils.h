#ifndef TIMESTAMP_UTILS_H
#define TIMESTAMP_UTILS_H

#include <string>

class TimestampUtils {
public:
    static std::string nanosecondsToDateTime(
        long long timestampNanoseconds
    );
};

#endif