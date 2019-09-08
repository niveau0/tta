#include <effects.h>
#include <macros.h>
#include <properties.h>

static mapping cursed_players;

static nomask void
create()
{
    if (IS_CLONE)
	destruct_object();

    cursed_players = ([ ]);
}

/*
 * Function name: query_level
 * Description:   gives curse level
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
 * Description:   gives name of subdesc
 * Returns:       string
 */
string
query_subdesc_id()
{
    return "_base_";
}

/*
 * Function name: curse_player
 * Description:   curse a given player
 * Arguments:     tp - the player that should become cursed
 * Returns:       1 if cursed, else 0
 */
nomask int
curse_player(object tp)
{
    if (!tp)
	return 0;

    if (cursed_players[tp])
    {
	cursed_players[tp] = 1;
	tp->add_effect(CURSE_EFFECT, MASTER, 1);
	return 1;
    }
    cursed_players[tp] = 1;
    tp->add_effect(CURSE_EFFECT, MASTER, 1);
    if (function_object("subdesc", this_object()))
	tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("curse_effect", query_interval(), tp);
    return 1;
}

/*
 * Function name: restart_effect
 * Description:   start curse after login
 * Arguments:     tp - player object
 *                count - current curse state
 */
nomask void
restart_effect(object tp, int count)
{
    if (!tp || count < 1)
	return;

    cursed_players[tp] = 1;
    tp->add_effect(CURSE_EFFECT, MASTER, count);
    tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("curse_effect", query_interval(), tp);
}

/*
 * Function name: remove_curse
 * Description:   remove a curse
 * Arguments:     tp - the player to heal
 */
void
remove_curse(object tp)
{
    if (!tp)
	return;
    cursed_players[tp] = 0;
    tp->remove_subdesc(query_subdesc_id());
    tp->remove_effect(CURSE_EFFECT, MASTER);
}

/*
 * Function name: query_cursed
 * Description:   gives all cursed players
 * Returns:       mapping
 */
nomask mapping
query_infected()
{
    return cursed_players;
}
