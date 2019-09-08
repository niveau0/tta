#ifndef RACE_H
#define RACE_H

/* race sizes, these should be used e.g. to set the sizes for armor */
#define HOBBIT_MIN 20
#define HOBBIT_MAX 40
#define DWARF_MIN  38
#define DWARF_MAX  55
#define HUMAN_MIN  53
#define HUMAN_MAX  76
#define ELF_MIN    72
#define ELF_MAX    93

#define RACE_SIZES \
([ \
 "articman" : ({ HUMAN_MIN, HUMAN_MAX }), \
 "dunedan"  : ({ HUMAN_MIN, HUMAN_MAX }), \
 "dwarf"    : ({ DWARF_MIN, DWARF_MAX }), \
 "halfelf"  : ({ ELF_MIN,   ELF_MAX }), \
 "hobbit"   : ({ HOBBIT_MIN, HOBBIT_MAX }), \
 "noldo"    : ({ ELF_MIN, ELF_MAX }), \
 "northman" : ({ HUMAN_MIN, HUMAN_MAX }), \
 "sinda"    : ({ ELF_MIN, ELF_MAX }), \
 "variag"   : ({ HUMAN_MIN, HUMAN_MAX }), \
 "woodelf"  : ({ ELF_MIN, ELF_MAX }), \
    ])

/*
 * RACES
 * all races available for mortal players
 */
#define RACES ({ "arcticman", "dunedan", "dwarf", "halfelf", "hobbit",\
		 "noldo", "northman", "sinda", "variag", "woodelf", })

#define CO_MOD           0 /* constitution modifier */
#define AG_MOD		 1 /* agility modifier */
#define SD_MOD		 2 /* self-discipline modifier */
#define ME_MOD		 3 /* memory modifier */
#define RE_MOD		 4 /* reasoning modifier */
#define ST_MOD		 5 /* strength modifier */
#define QU_MOD		 6 /* quickness modifier */
#define PR_MOD		 7 /* presence modifier */
#define EM_MOD		 8 /* empathy modifier */
#define IN_MOD		 9 /* intution modifier */
#define AP_MOD		10 /* appearance modifier */
#define RRM_ESS 	11 /* resist ess */
#define RRM_CHA  	12 /* resist cha */
#define RRM_MEN	        13 /* resist men */
#define RRM_POISON	14 /* resist poison */
#define RRM_DISEASE	15 /* resist disease */
#define SOUL_DEP	16 /* soul departure time */
#define STAT_DETORT	17 /* stat detortion value */
#define RECOVERY	18 /* */
#define HIT_DICE	19 /* */
#define MAX_HITS	20 /* maximum hitoints that can be reached */
#define HEIGHT_MOD      21
#define WEIGHT_MOD      22

/* Mapping holds the standard modifiers for each race. The attributes are: \
 * CO_MOD, AG_MOD, SD_MOD, ME_MOD, RE_MOD, \
 * ST_MOD, QU_MOD, PR_MOD, EM_MOD, IN_MOD, AP_MOD, \
 * RRM_ESS, RRM_CHA, RRM_MEN, RRM_POISON, RRM_DISEASE, \
 * SOUL_DEP, STAT_DETORT, RECOVERY, HIT_DICE, MAX_HITS, HEIGHT, WEIGHT */
#define RACE_MODIFIER ([\
"ainu"         : ({  50,  50,  50,  50,  50,                 \
		     50,  50,  50,  50,  50,  50,            \
		     15,  15,  15,  50, 100,                 \
		    100,   0,  20,  10, 999, 1.40, 0.85, }), \
\
"arcticman"    : ({  10,   0,   5,   0,   5,                 \
                      5,   0,  -5,   0,   5,   0,            \
		      0,   0,   0,   0,  15,                 \
		     12,   0,   7,   8, 120, 0.85, 1.00, }), \
\
"dunedan"      : ({  10,  -5,   0,   0,   0,                 \
                     10,  -5,  10,   0,   0,   0,            \
		     -5,   5,  -5,   0,   0,                 \
		     10,   0,   7,  10, 150, 1.15, 1.00, }), \
\
"dwarf"        : ({  15,  -5,   5,   0,   0,                 \
                      5,  -5, -10, -10,   0,   0,            \
		     40,   0,  40,  20,  15,                 \
		     18,  -1,   5,  10, 120, 0.75, 1.20, }), \
\
"halfelf"      : ({   5,   5, -10,   0,   0,                 \
                      5,  10,  10,   0,   0,   0,            \
		     -5,  -5,  -5,   0,  50,                 \
		      3,   0,   7,  10, 150, 1.20, 0.85, }), \
\
"hobbit"       : ({  15,  15, -10,   0,   0,                 \
		    -20,  10, -15,  -5,   0,   0,            \
		     50,   0,  40,  30,  15,                 \
		     18,  -1,   5,   8,  80, 0.60, 1.10, }), \
\
"noldo"        : ({   0,   5, -20,   5,   0,                 \
                      0,  15,  15,   5,   0,   0,            \
		     -5,  -5,  -5,  10, 100,                 \
		      1,   6,   3,  10, 120, 1.25, 0.80, }), \
\
"northman"     : ({  10,  -5,  -5,   0,   0,                 \
                     10,   0,   5,  -5,   0,   0,            \
		      0,   0,   0,   0,  10,                 \
		     12,   0,  10,  10, 150, 1.15, 0.90, }), \
\
"sinda"        : ({   5,   5, -20,   5,   0,                 \
                      5,  10,  10,   5,   0,   0,            \
		     -5,  -5,  -5,  10, 100,                 \
		      2,   4,  20,   8, 110, 1.22, 0.85, }), \
\
"variag"       : ({   5,   0,   5,   0,   0,                 \
                      5,   0,   5,   0,   0,   0,            \
		      0,   0,   0,   0,   5,                 \
		     10,   0,   8,  10, 130, 1.05, 0.95, }), \
\
"woodelf"       : ({   0,  10, -20,   5,   0,                \
                      0,   5,   5,   5,   0,   0,            \
		     -5,  -5,  -5,  10, 100,                 \
		      3,   2,  15,   8, 100, 1.20, 0.80, }), \
])

#endif
