#ifndef	_RTC_H
#define	_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

struct UTC_TIME {
    batman_u16_t year : 12,      // max 4095
        month : 4;      // max 15
    batman_u8_t  day : 5,        // max 31
        weekday : 3;    // max 7
    batman_u8_t hour;
    batman_u8_t minute;
    batman_u8_t second;
};

struct UTC_TIME unix32_to_UTC(uint32_t unix_time);

uint32_t UTC_to_unix32(struct UTC_TIME utc_time);

#ifdef __cplusplus
}
#endif

#endif
