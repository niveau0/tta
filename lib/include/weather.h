#ifndef WEATHER_H
#define WEATHER_H

/*
 * weather definitions, for correct usage see <config.h> WEATHER_MASTER file
 */

/* climates */
#define CLIMATE_MEDIUM		1
#define CLIMATE_MEDIUM_HUMID    2
#define CLIMATE_MEDIUM_ARID     3
#define CLIMATE_HOT		4
#define CLIMATE_HOT_HUMID	5
#define CLIMATE_HOT_ARID	6
#define CLIMATE_COLD	        7	
#define CLIMATE_COLD_HUMID	8
#define CLIMATE_COLD_ARID	9
#define CLIMATE_POLAR      	10

#define WINDDESC \
({ \
       "There is absolutely no wind at the moment.",\
       "You can hardly trace any wind.",\
       "Now and then a gentle breeze whirls around your nose.",\
       "Sometimes gusts of wind stripe your body.",\
       "A strong breeze blows around here.",\
       "Strong blasts of wind are blowing around.",\
       "The storm is unbelievable. Find some place to hide!",\
       })       

#endif
