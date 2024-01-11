#include <stdint.h>
#include <stdbool.h>

#define SECOND                      1UL
#define SECONDS_IN_MINUTE           (60 * SECOND)
#define SECONDS_IN_HOUR             (60 * SECONDS_IN_MINUTE)
#define SECONDS_IN_DAY              (24 * SECONDS_IN_HOUR)
#define MINUTES_IN_HOUR             60
#define TIME_UNIT                   60

#define UNIX_EPOCH_YEAR             1970
#define CLOSEST_FAKE_LEAP_YEAR      2102 // 2100 is nonleap year
#define UNIX_EPOCH_YEAR_WEEKDAY     4    // Thursday

#define DAYS_IN_NONLEAP_YEAR        365
#define DAYS_IN_LEAP_YEAR           366
#define EVERY_4YEARS_DAYS           (DAYS_IN_NONLEAP_YEAR * 3 + DAYS_IN_LEAP_YEAR)
#define DAYS_OF_WEEK                7

#define ARRAY_SIZE(array)           (sizeof(array)/sizeof(array[0]))

struct UTC_TIME {
    uint16_t year : 12,	// max 4095
        month : 4;     	// max 15
    uint8_t  day : 5,  	// max 31
        weekday : 3;   	// max 7
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

static bool is_leap_year(uint16_t year)
{
    if (((year % 4) == 0) && ((year % 100) != 0)) {
        return 1;
    } else if ((year % 400) == 0) {
        return 1;
    }

    return 0;
}

struct UTC_TIME unix32_to_UTC(uint32_t unix_time)
{
    uint8_t days_per_month[12] = {
        /*
        1   2   3   4   5   6   7   8   9   10  11  12
        */
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    struct UTC_TIME utc = {0};
    uint32_t pass_days, pass_days_cnt, pass_days_cnt_next;
    uint8_t pass_4years_cnt;
    uint16_t basic_4multiple_year;
    uint16_t CurrentYear_PassDays, CurrentDay_PassMinutes;
    uint32_t CurrentDay_PassSeconds;

    pass_days = unix_time / SECONDS_IN_DAY; // passed days since UNIX_EPOCH_YEAR
    pass_4years_cnt = pass_days / EVERY_4YEARS_DAYS; // passed how many 4 years
    basic_4multiple_year = (pass_4years_cnt * 4) + UNIX_EPOCH_YEAR; // next 4 year basic, base on UNIX_EPOCH_YEAR
    pass_days_cnt = pass_4years_cnt * EVERY_4YEARS_DAYS; // passed day of pass_4years_cnt
    if (basic_4multiple_year >= CLOSEST_FAKE_LEAP_YEAR) {
        pass_days_cnt--;
    }

    //< get year
    for (uint16_t i = basic_4multiple_year; ; i++) {
        pass_days_cnt_next = is_leap_year(i)? \
                            (pass_days_cnt + DAYS_IN_LEAP_YEAR) : (pass_days_cnt + DAYS_IN_NONLEAP_YEAR);
        if (pass_days_cnt_next > pass_days) {
            utc.year = i;
            break;
        }
        pass_days_cnt = pass_days_cnt_next;
    }

    //< get month
    CurrentYear_PassDays = pass_days - pass_days_cnt;
    pass_days_cnt = pass_days_cnt_next = 0;
    if (is_leap_year(utc.year)) {
        days_per_month[1]++; // leap month of February is 29 days
    }
    for (uint8_t i = 0; i < ARRAY_SIZE(days_per_month); i++) {
        pass_days_cnt_next += days_per_month[i];
        if (pass_days_cnt_next > CurrentYear_PassDays) {
            utc.month = i + 1;
            break;
        }
        pass_days_cnt = pass_days_cnt_next;
    }

    //< get day
    utc.day = CurrentYear_PassDays - pass_days_cnt + 1;

    //< get hour:minute:second
    CurrentDay_PassSeconds = unix_time - (pass_days * SECONDS_IN_DAY);
    CurrentDay_PassMinutes = CurrentDay_PassSeconds / SECONDS_IN_MINUTE;
    utc.hour = CurrentDay_PassMinutes / MINUTES_IN_HOUR;
    utc.minute = CurrentDay_PassMinutes % TIME_UNIT;
    utc.second = CurrentDay_PassSeconds % TIME_UNIT;

    //< get weekday
    utc.weekday = (pass_days + UNIX_EPOCH_YEAR_WEEKDAY) % DAYS_OF_WEEK;

    return utc;
}

uint32_t UTC_to_unix32(struct UTC_TIME utc_time) {
    uint32_t unix_time = 0;

    // Calculate days since UNIX epoch
    uint16_t year = utc_time.year;
    uint32_t days = (year - UNIX_EPOCH_YEAR) * DAYS_IN_NONLEAP_YEAR;

    // Account for leap years
    for (uint16_t y = UNIX_EPOCH_YEAR; y < year; ++y) {
        if (is_leap_year(y)) {
            days++;
        }
    }

    // Adjust days based on months passed
    uint8_t days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (is_leap_year(year)) {
        days_per_month[1]++; // Adjust for February in a leap year
    }

    for (int i = 0; i < utc_time.month - 1; ++i) {
        days += days_per_month[i];
    }

    // Add days passed in the current month
    days += (utc_time.day - 1);

    // Convert days to seconds
    unix_time = days * SECONDS_IN_DAY;

    // Add time components to the Unix32 time
    unix_time += (utc_time.hour * SECONDS_IN_HOUR);
    unix_time += (utc_time.minute * SECONDS_IN_MINUTE);
    unix_time += utc_time.second;

    return unix_time;
}
