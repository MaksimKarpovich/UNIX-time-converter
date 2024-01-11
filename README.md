## Conversion

 `Unix time` :arrow_right: `UTC` | `UTC` :arrow_right: `Unix time`
:-:|:-:
:heavy_check_mark: | :heavy_check_mark:

## Usage

```C
#include "time_converter.h"

int main(int argc, char const *argv[])
{
    struct UTC_TIME cur_utc;
    uint32_t unix_time = 0;

    cur_utc = unix32_to_UTC(0);
    printf("unix_time = %d is equal UTC's: %d/%d/%d %02d:%02d:%02d, weekday %d\r\n",
              unix_time,
              cur_utc.year, cur_utc.month, cur_utc.day,
              cur_utc.hour, cur_utc.minute, cur_utc.second,
              cur_utc.weekday);

    unix_time = 1704982823;
    cur_utc = unix32_to_UTC(unix_time);
    printf("unix_time = %d is equal UTC's: %d/%d/%d %02d:%02d:%02d, weekday %d\r\n",
              unix_time,
              cur_utc.year, cur_utc.month, cur_utc.day,
              cur_utc.hour, cur_utc.minute, cur_utc.second,
              cur_utc.weekday);

    cur_utc.year = 2027;
    cur_utc.month = 4;
    cur_utc.day = 7;
    cur_utc.hour = 12;
    cur_utc.minute = 34;
    cur_utc.second = 0;
    cur_utc.weekday = 3;
    unix_time = UTC_to_unix32(cur_utc);

    printf("UTC time: %d/%d/%d %02d:%02d:%02d, weekday %d is equal unix_time = %d\r\n",
              cur_utc.year, cur_utc.month, cur_utc.day,
              cur_utc.hour, cur_utc.minute, cur_utc.second,
              cur_utc.weekday,
              unix_time);
    return 0;
}
```
Console output:

```C
unix_time = 0 is equal UTC's: 1970/1/1 00:00:00, weekday 4
unix_time = 1704982823 is equal UTC's: 2024/1/11 14:20:23, weekday 4
UTC time: 2027/4/7 12:34:00, weekday 3 is equal unix_time = 1807101240
```

## Thanks

The library was inspired by [this repository](https://github.com/hhy-ccj/Unix_timestamp_converter/tree/master)

If the library was useful to you, then I will be glad to receive a star :star: