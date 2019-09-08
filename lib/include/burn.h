#ifndef BURN_H
#define BURN_H

/* Ratio defining necessary energy for 1 lightunit */
#define ENERGY_TO_LIGHT 0.3

/* Minimum energy necessary to damage material */
#define BURN_THRESHOLD 0  
/* Energy produced by burning 1 gram of material */
#define BURN_ENERGY    1  
/* Time in seconds until 1 gram is burned up */
#define BURN_TIME      2  
/* Energy lost during burning for BURN_TIME in % (radiated into room, or whatever) */
#define ENERGY_LOSS    3  

/* Current values are just made for test purposes, changes might be necessary */
#define BURN_TABLE \
([                                                          \
    M_SILK:    ({ 60,   75,  1, 15 }),                      \
    M_LINEN:   ({ 90,   105, 1, 15 }),                      \
    M_COTTON:  ({ 80,   95,  1, 15 }),                      \
    M_GLASS:   ({ 800,  2,   1, 8  }),                      \
    M_STONE:   ({ 1500, 5,   1, 9  }),                      \
    M_BONE:    ({ 900,  7,   1, 10 }),                      \
    M_IVORY:   ({ 1100, 6,   1, 9  }),                      \
    M_PAPER:   ({ 70,   100, 1, 20 }),                      \
    M_LEATHER: ({ 100,  110, 1, 12 }),                      \
    M_EARTH:   ({ 700,  10,  1, 10 }),                      \
    M_AIR:     ({ 2000, 1,   1, 2  }),                      \
    M_LIQUID:  ({ 100,  2,   1, 3  }),                      \
    M_ICE:     ({ 0,    2,   1, 4  }),                      \
    M_WOOL:    ({ 75,   90,  1, 15 }),                      \
    M_WAX:     ({ 40,   45,  1, 11 }),                      \
    M_FOOD:    ({ 50,   58,  1, 13 }),                      \
    M_FLESH:   ({ 40,   10,  1, 14 }),                      \
])
#endif
