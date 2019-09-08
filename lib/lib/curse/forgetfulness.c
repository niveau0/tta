inherit "/base/curse";

#include <effects.h>
#include <living.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL random(100)+100
#define LEVEL    30


static nomask void
curse_effect(object tp)
{
    int i;
    string *ind;
    mapping seen_players;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    call_out("curse_effect", INTERVAL, tp);

    seen_players = tp->query_remembered();

    ind = map_indices(seen_players);
    
    for (i=sizeof(ind);i--;)
    {
	if(!tp->query_met(ind[i]))
	    ind[i] = 0;
    }
    
    ind -= ({ 0 });

    if (!sizeof(ind))
	return;

    i = random(sizeof(ind));

    tp->remove_remembered(ind[i]);
}


/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_forgetfulness_";
}

/*
 * Function name: query_level
 * Description:   gives curse level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives curse description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "let you forget a persons name";
}

/*
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
nomask int
query_interval()
{
    return INTERVAL;
}
