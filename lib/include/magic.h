#ifndef MAGIC_H
#define MAGIC_H

#define CHA  1
#define MEN  2
#define ESS  4
#define ARC  7
#define REALMS ({ "cha", "ess", "men", "arc" })
#define REALM_ID ([ "cha":CHA, "ess":ESS, "men":MEN, "arc":ARC ]) 
#define ID_REALM ([ CHA:"cha", ESS:"ess", MEN:"men", ARC:"arc" ]) 
#define POWER_SYLLABLES ({ "en", "rec", "di", "ter", "qar", "olc" })
#define OVERCAST_SYLLABLES ({ "sic", "mor", "rip" })

#define SPELL_DIR     "/lib/spell"
#define SPELL_FUMBLES "/base/fumble/spells"
#define CAST_RND      5 /* how long are 'cast or prepare' rounds */

/* spell types */
#define SP_DEFENSIVE       1
#define SP_ELEMENTAL       2
#define SP_FORCE           4
#define SP_HEALING         8
#define SP_MENTALATTACK   16
#define SP_PASSIVE        32
#define SP_SUBCONSCIOUS   64
#define SP_INSTANTLY     128

/* special duration */
#define SP_CONCENTRATE   -1
#define SP_PERMANENT     -2
#define SP_NONE          -3
#define SP_LEVEL_TIME    -4

/* special range */
#define SP_SELF          -1
#define SP_TOUCH         -2 

/* special target number */
#define SP_LEVEL_TARGETS -1
#define SP_DIRECTION     -2
#define SP_LEVEL_RANGE   -3

/* target types */
#define SP_ALL_TARGETS    1
#define SP_ALL_LIVINGS    2
#define SP_ALL_OBJECTS    3

/* array indices in spell data array, special values see above  */
#define SP_TYPE           0 /* sort of the spell */
#define SP_DURATION       1 /* spell duration in seconds if not special */
#define SP_RANGE          2 /* spell range in inch */
#define SP_TARGETS        3 /* number of possible spell targets */
#define SP_TARGET_TYPE    4 /* what kind of targets are allowed */
#define SP_MAGIC_LANGUAGE 5 /* language correlated with the spell */

#define SP_DATA_SIZE      6

#endif
