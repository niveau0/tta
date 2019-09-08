#ifndef LIQUID_H
#define LIQUID_H

#define LIQUID_EFFECT(t, a, p) \
"/base/liquid/liquid"->drink_liquid(t, a, p);

/* alcohol */
#define LIQ_ALCOHOL_FIRST  1
#define LIQ_BEER 	   1
#define LIQ_WINE  	   2
#define LIQ_ALCOHOL_LAST   2

/* potion */
#define LIQ_POTION_FIRST   100
#define LIQ_BAALAK         100
#define LIQ_BURSTHELAS     101
#define LIQ_ELDAANA        102
#define LIQ_FEK            103
#define LIQ_FEROL          104
#define LIQ_MENELAR        105
#define LIQ_QUILMUFUR      106
#define LIQ_REGLEN	   107
#define LIQ_REWK	   108
#define LIQ_THURL          109
#define LIQ_DRAAK          110
#define LIQ_KARAD          111
#define LIQ_POTION_LAST    111


/* other */
#define LIQ_OTHER_FIRST    300
#define LIQ_BREWING        300
#define LIQ_JUICE	   301
#define LIQ_MILK 	   302
#define LIQ_OIL   	   303
#define LIQ_SOUP           304
#define LIQ_VINEGAR 	   305
#define LIQ_WATER 	   306
#define LIQ_HOLY_WATER 	   307
#define LIQ_OTHER_LAST     307

/* poison */
#define LIQ_POISON_FIRST   500
#define LIQ_POISON_LAST    500

#endif
