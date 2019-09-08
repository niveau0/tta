#ifndef MUDTIME_H
#define MUDTIME_H

#define TIME_FILE "/system/mudtime"

#define TIME_SCALE 6

#define DAYLIGHT   36000  /* view distance at day [inch] */
#define NIGHTLIGHT 1800   /* view distance at night [inch] */

#define DAWN       5
#define DAYTIME    7
#define FORENOON   9
#define MIDDAY     12
#define AFTERNOON  15
#define DUSK       20
#define NIGHTTIME  23

#define DAWNDESC \
"First sunbeams reach your eyes, the night seems to be over.\n"
#define DAYDESC \
"Dawn has done its work, the sun starts travelling westwards.\n"
#define FORENOONDESC \
"You guess it's now forenoon.\n"
#define MIDDAYDESC \
"The warm bright star finished half of its daily way.\n"
#define AFTERNOONDESC \
"It's now afternoon, the shadows grow longer.\n"
#define DUSKDESC \
"Last rays of sunlight try to escape the horizon.\n"
#define NIGHTDESC \
"Night falls over the land.\n";

#define MOON_PHASE_1 1 /* no moon */
#define MOON_PHASE_2 2 /* one quarter */
#define MOON_PHASE_3 3 /* half */
#define MOON_PHASE_4 4 /* 3 quarters */
#define MOON_PHASE_5 5 /* full moon */

/* value postion in time array (see time_get_int()) */
#define YEAR_POS       0
#define MONTH_POS      1
#define DAY_POS        2
#define HOUR_POS       3
#define MINUTE_POS     4
#define SECOND_POS     5
#define WEEKDAY_POS    6

/* seasons */
#define SPRING      1
#define SUMMER      2
#define AUTUMN      3
#define WINTER      4

#define YEAR            call_other(TIME_FILE,"time_get_int")[YEAR_POS]
#define MONTH           call_other(TIME_FILE,"time_get_int")[MONTH_POS]
#define DAY             call_other(TIME_FILE,"time_get_int")[DAY_POS]
#define HOUR            call_other(TIME_FILE,"time_get_int")[HOUR_POS]
#define MINUTE          call_other(TIME_FILE,"time_get_int")[MINUTE_POS]
#define SECOND          call_other(TIME_FILE,"time_get_int")[SECOND_POS]
#define WEEKDAY         call_other(TIME_FILE,"time_get_int")[WEEKDAY_POS]
#define TIMEDESC        call_other(TIME_FILE,"time_get_desc", 1)   
#define TIMEARRAY(mod)  call_other(TIME_FILE,"time_get_int", mod)   
#define TIMESTRING(a)   call_other(TIME_FILE,"time_get_string", a)   
#define DATEDESC        call_other(TIME_FILE,"time_get_desc", 0)   
#define NIGHT           call_other(TIME_FILE,"is_it_night")
#define SEASON          call_other(TIME_FILE,"season")

#define CONV_REALTIME(t) call_other(TIME_FILE,"conv_real_time", t)

#endif
