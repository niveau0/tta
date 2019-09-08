inherit "../../men_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>
#define rounds_relieved 1

string
query_desc()
{
    return "A stun relief spell that lets you overcome the effect " +
	"of stun more quickly.";
}

void
spell_effect(int level, object *tar, object tp)
{

    int     i, max, *array_part1, *array_part2;
    mapping map;
    mixed   ind;

    
    if (!sizeof(tar))
	return;

    map = tar[0]->cb_bonus_map()[CB_STUNNED];
    ind = map_indices(map);
    if (sizeof(ind)==0)
    {
	tar[0]->catch_tell("You are not stunned.\n");
        return;
    }
    tar[0]->catch_tell("You feel magic floating through your body.\n");
    for (i=sizeof(ind), array_part1=allocate(i), array_part2=allocate(i); i--;)
    {
	if (!intp(ind[i]) || ind[i] < 1)
        continue;
	array_part1[i] = CB_STUNNED;
	array_part2[i] = ind[i];
	if (ind[i] > max)
	    max = ind[i];
    }
    tar[0]->cb_remove_boni(array_part1, array_part2);
    if ((max = max - rounds_relieved * CB_ROUND)<=0)
	return;
    tar[0]->cb_add_boni(({ CB_STUNNED }), ({ ([ max : 1 ]) }));

}

int
query_level()
{
    return 6;
}

int *
query_spell_data()
{
    return
	({
	    (SP_HEALING | SP_INSTANTLY | SP_SUBCONSCIOUS),
	    SP_NONE,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
