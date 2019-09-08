#include <effects.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

static mapping infected_players;

static nomask void
create()
{
    if (IS_CLONE)
	destruct_object();
    
    infected_players = ([ ]);
}

/*
 * Function name: query_level
 * Description:   gives disease level
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
    return "base";
}

/*
 * Function name: infect_player
 * Description:   infect a given player
 * Arguments:     tp - the player that should become ill
 *                rrmod - resistance modifier (positiv -> easier)
 * Returns:       1 if infected, else 0
 */
nomask varargs int
infect_player(object tp, int rrmod)
{
    if (!tp)
	return 0;

    if (tp->resistance_roll(RR_DISEASE, query_level(), rrmod) > 0)
	return 0;
    if (infected_players[tp])
    {
	infected_players[tp] = 1;
	tp->add_effect(DISEASE_EFFECT, MASTER, 1);
	return 1;
    }
    infected_players[tp] = 1;
    tp->add_effect(DISEASE_EFFECT, MASTER, 1);
    if (function_object("subdesc", this_object()))
	tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("disease_effect", query_interval(), tp);
    return 1;
}


/*
 * Function name: spread_disease
 * Description:   check if players around will be affected by the disease
 * Arguments:     p - infection per mille (1000 will affect all people)
 *                tp - living that is spreading the virus
 */
nomask void
spread_disease(int p, object tp)
{
    int    i, *co, *range;
    object *people;
    
    if (!tp || !p)
	return;
    co = tp->query_coordinates()[..];
    co[0] -= 150;
    co[1] -= 150;
    co[2] -= 150;
    range = ({ 300, 300, 300 });
    people = I(tp, 2) - ({ tp });
    for (i=sizeof(people); i--;)
    {
	if (random(1000) < p && in_range(co, people[i]->query_coordinates(),
					 range))
	    infect_player(people[i], 0);
    }
}

/*
 * Function name: restart_effect
 * Description:   start disease after login
 * Arguments:     tp - player object
 *                count - current disease state
 */
nomask void
restart_effect(object tp, int count)
{
    if (!tp || count < 1)
	return;
    
    infected_players[tp] = 1;
    tp->add_effect(DISEASE_EFFECT, MASTER, count);
    tp->add_subdesc(query_subdesc_id(), this_object());
    call_out("disease_effect", query_interval(), tp);
}

/*
 * Function name: remove_disease
 * Description:   remove a disease
 * Arguments:     tp - the player to heal
 */
void
remove_disease(object tp)
{
    if (!tp)
	return;
    infected_players[tp] = 0;
    tp->remove_subdesc(query_subdesc_id());
    tp->remove_effect(DISEASE_EFFECT, MASTER);
}

/*
 * Function name: query_infected
 * Description:   gives all infected players
 * Returns:       mapping
 */
nomask mapping
query_infected()
{
    return infected_players;
}
