inherit "../../cha_base";

#include <effects.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Target can be healed from any poison. Power manipulates the " +
	"strenght of the caster at the 'duel': caster vs. poison.";
}


void
spell_effect(int level, object *tar, object tp, string extra)
{
    int p_lev, i, j, sz;
    string *poison;
    mapping poi;
    
    if ( sizeof(tar))
    {
	poi = tar[0]->query_effects()[POISON_EFFECT];
	
	if(!poi || !(sz=map_sizeof(poi)))
	{
	    tp->catch_tell("Nothing happens.\n");
	    return;
	}
	poison = map_indices(poi);
	for (i=sizeof(poison); i--;)
	{
	    p_lev = poison[i]->query_level();
	    if ((level-34) * 8 > (random(p_lev)))
	    {
		poison[i]->remove_poison(tar[0]);
		j++;
	    }
	}
	if(j==sz)
	    tp->catch_tell("You think you had a full success.\n");
	else if (j)
	    tp->catch_tell("You think you had some success.\n");
	else
	    tp->catch_tell("Nothing happens.\n");
    }
}

int
query_level()
{
    return 35;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_NONE,
	    100,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_CONSCIOUSNESS,
	});
}
