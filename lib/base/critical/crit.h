#include <combat.h>
#include <event.h>
#include <effects.h>
#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <slots.h>
#include <state.h>
#include <properties.h>

/* tell macros */
#define AOBS(m, f, t, d) ao->catch_msg(({ m }), f, t, d)
#define ATBS(m, f, t, d) att->catch_msg(BS(m, 77, 0), f, t, d)
#define AO(m, f, t, d)   ao->catch_msg(m, f, t, d)
#define AT(m, f, t, d)   att->catch_msg(m, f, t, d)
#define AOB(i, v)        ao->cb_add_boni(i, v)
#define ATB(i, v)        att->cb_add_boni(i, v)
#define TW(m, o, a)      tell_watcher(m, o, a)

static int crit_head(mixed hloc, object ao, object att, int step);
static int crit_torso(mixed hloc, object ao, object att, int step);
static int crit_other(mixed hloc, object ao, object att, int step);

static nomask void
tell_watcher(mixed str, object ao, object att)
{
    int    i;
    object *ob;
    
    ob = LISTEN(E(ao)) - ({ ao, att });
    for (i=sizeof(ob); i--;)
	if (ob[i]->query_prop(LIVE_SEE_FIGHT))
	    ob[i]->catch_msg(str, att, MSG_SEE);
}

/*
 * Function name: critical
 * Description:   evalutate a slash critical for a given hitlocation
 * Arguments:     hloc - hit location id
 *                ao - attack object
 *                att - attacking player
 *                over - overcoming of threshold
 * Returns:       integer, extra damage
 */
int
critical(mixed hloc, object ao, object att, int over)
{
    int step;
    
    if (ao->query_prop(LIVE_NO_CRITICAL))
	return 0;
    
    switch (random(129))
    {
      case 0..10:    step = 0;  break;
      case 11..21:   step = 1;  break;
      case 22..31:   step = 2;  break;
      case 32..41:   step = 3;  break;
      case 42..50:   step = 4;  break;
      case 51..59:   step = 5;  break;
      case 60..67:   step = 6;  break;
      case 68..75:   step = 7;  break;
      case 76..82:   step = 8;  break;
      case 83..89:   step = 9;  break;
      case 90..95:   step = 10; break;
      case 96..101:  step = 11; break;
      case 102..106: step = 12; break;
      case 107..111: step = 13; break;
      case 112..115: step = 14; break;
      case 116..119: step = 15; break;
      case 120..122: step = 16; break;
      case 123..125: step = 17; break;
      case 126..127: step = 18; break;
      case 128:      step = 19; break;
      default:       error("Wrong critical step");
    }
    step += over;
    
    switch (SLOT_TYPE(hloc[HITLOC_SLOT]))
    {
      case SLT_HEAD:
	  return crit_head(hloc, ao, att, step);
      case SLT_CHEST:
      case SLT_ABDOMEN:
	  return crit_torso(hloc, ao, att, step);
      default:
	  return crit_other(hloc, ao, att, step);
    }
}
