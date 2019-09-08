inherit "../../cha_base";

#include <effects.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Target can be healed from any disease. Power manipulates the " +
	"strenght of the caster at the 'duel': caster vs. poison.";
}


void
spell_effect(int level, object *tar, object tp, string extra)
{
    int lev, i, j, sz;
    string *disease;
    mapping dis;
    
    if ( sizeof(tar))
    {
	dis = tar[0]->query_effects()[DISEASE_EFFECT];
	
	if(!dis || !(sz=map_sizeof(dis)))
	{
	    tp->catch_tell("Nothing happens.\n");
	    return;
	}
	disease = map_indices(dis);
	for (i=sizeof(disease); i--;)
	{
	    lev = disease[i]->query_level();
	    if ((level-35) * 8 > (random(lev)))
	    {
		disease[i]->remove_disease(tar[0]);
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
    return 36;
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
