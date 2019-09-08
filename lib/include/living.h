#ifndef LIVING_H
#define LIVING_H

#define STATUE_ROOM "/config/login/statue_room"

/*
 * STAT_xxx - All stats a living could have
 */
#define STAT_FIRST 0            /* index of first stat */

#define STAT_CO 0		/* constitution */
#define STAT_AG 1		/* agility */
#define STAT_SD 2		/* self-discipline */
#define STAT_ME 3		/* memory */
#define STAT_RE 4		/* reasoning */
#define STAT_ST 5		/* strength */
#define STAT_QU 6		/* quickness */
#define STAT_PR 7		/* presence */
#define STAT_EM 8		/* empathy */
#define STAT_IN 9		/* intuition */
#define STAT_AP 10		/* appearance */

#define STAT_LAST 10            /* index of last stat */

/*
 * STAT_DESC - description for each stat
 */
#define STAT_DESC \
({ "constitution", "agility", "self-discipline", "memory", "reasoning",\
     "strength", "quickness", "presence", "empathy", "intuition", \
     "appearance", })
/*
 * STAT_ABBREV - abbrevations
 */
#define STAT_ABBREV \
({ "co", "ag", "sd", "me", "re", "st", "qu", "pr", "em", "in", "ap" })

#define G_MALE   0
#define G_FEMALE 1
#define G_NEUTER 2

#endif
