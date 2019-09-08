#include <effects.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

static nomask void poison_effect(object tp);
static mapping poisoned_players;

static nomask void
create()
{
    if (IS_CLONE)
	destruct_object();
    
    poisoned_players = ([ ]);
}

/*
 * Function name: query_level
 * Description:   gives poison level
 * Returns:       int
 */
int
query_level()
{
    return 0;
}

/*
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
int
query_interval()
{
    return 0;
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of sublocation
 * Returns:       string
 */
string
query_subdesc_id()
{
    return "_base_";
}

/*
 * Function name: renew_poison(object tp)
 * Description:   do something if the living gets again poisoned while still
 *                suffering from _this_ poison
 * Arguments:     tp - poisoned living
 */
static void
renew_poison(object tp)
{
}

/*
 * Function name: poison_player
 * Description:   poison a given player
 * Arguments:     tp - the player that should become poisoned
 *                rrmod - resistance modifier (positiv -> easier)
 * Returns:       1 if poisoned, else 0
 */
nomask varargs int
poison_player(object tp, int rrmod)
{
    mapping e;
    
    if (!tp)
	return 0;

    if (tp->resistance_roll(RR_POISON, query_level(), rrmod) > 0)
	return 0;
    if (poisoned_players[tp]) /* already poisoned with this poison */
    {
	renew_poison(tp);
	return 1;
    }
    poisoned_players[tp] = 1;
    tp->add_effect(POISON_EFFECT, MASTER, 1);
    if (function_object("subdesc", this_object()))
	tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("poison_effect", query_interval(), tp);
    return 1;
}

/*
 * Function name: restart_effect
 * Description:   start poison after login
 * Arguments:     tp - player object
 *                count - current poison state
 */
nomask void
restart_effect(object tp, int count)
{
    if (!tp || count < 1)
	return;
    
    poisoned_players[tp] = 1;
    tp->add_effect(POISON_EFFECT, MASTER, count);
    tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("poison_effect", query_interval(), tp);
}

/*
 * Function name: remove_poison
 * Description:   remove a poison
 * Arguments:     tp - the player to heal
 */
void
remove_poison(object tp)
{
    if (!tp)
	return;
    poisoned_players[tp] = 0;
    remove_call_out("poison_effect");
    tp->remove_subdesc(query_subdesc_id());
    tp->remove_effect(POISON_EFFECT, MASTER);
}

/*
 * Function name: query_poisoned
 * Description:   gives all poisoned players
 * Returns:       mapping
 */
nomask mapping
query_poisoned()
{
    return poisoned_players;
}
