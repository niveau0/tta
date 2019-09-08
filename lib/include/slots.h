#ifndef SLOTS_H
#define SLOTS_H

/* slot sublocations */
#define SUBLOC_WIELD  "_wield_"
#define SUBLOC_WORN   "_worn_"
#define SUBLOC_HOLD   "_hold_"
#define SUBLOC_ATTACH "_attach_"

/* SLT_ALL is used as a function argument where a list of slots or items
 * can be queried (e.g. query_wield() of /base/living.c) */
#define SLT_ALL         -1

/* slot types */
#define SLT_HEAD        100
#define SLT_EAR		200
#define SLT_EYE		300
#define SLT_NOSE 	400
#define SLT_TONGUE	500
#define SLT_CHEST	600
#define SLT_ARM	        700
#define SLT_HAND	800
#define SLT_ABDOMEN	900
#define SLT_LEG	        1000
#define SLT_FOOT	1100
#define SLT_MOUTH       1200
#define SLT_NECK        1300
#define SLT_BACK        1400

/* the following definitions are necessary modifiers for the slots position
 * e.g.
 *  SLT_SECOND(SLT_CHEST) is the second chest of a strange monster
 *  SLT_LEFT(SLT_ARM) is the left arm of some living
 */
#define SLT_LEFT(t)        (t + 1)
#define SLT_RIGHT(t)       (t + 2)
#define SLT_SECONDLEFT(t)  (t + 3)
#define SLT_SECONDRIGHT(t) (t + 4)

#define SLT_FIRST(t)	   (t + 1)
#define SLT_SECOND(t)	   (t + 2)
#define SLT_THIRD(t)	   (t + 3)
#define SLT_FOURTH(t)	   (t + 4)
#define SLT_FIFTH(t)	   (t + 5)
#define SLT_SIXTH(t)	   (t + 6)

/* macro to get the slots general type of a slot */
#define SLOT_TYPE(x)    (x / 100 * 100)

/* set_wound() types */
#define SLOT_UNUSABLE  1
#define SLOT_BROKEN    2
#define SLOT_SHATTERED 3
#define SLOT_LOST      4  /* worst state must be last define (see set_wound) */

/* set_layer() values */
#define LAYER_UNDERWEAR 1 /* must be worn first */
#define LAYER_CLOTH     2 
#define LAYER_CHAIN     3 /* chain armor can be worn between cloth and plate */
#define LAYER_PLATE     4
#define LAYER_CAPE      5 /* capes are worn around plate */
#define LAYER_ABOVE_ALL 6 /* like backpacks, etc. */

#endif
