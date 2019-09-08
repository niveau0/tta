inherit "../../cha_base";

#include <effects.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Target will be healed from all diseases and poisons.";
}


void
spell_effect(int level, object *tar, object tp, string extra)
{
    int i;
    string *effects;
    mapping eff;
    
    if (sizeof(tar))
    {
	eff = tar[0]->query_effects()[DISEASE_EFFECT];
	if(!eff || !map_sizeof(eff))
	    i=0;
	else
	{
	    effects = map_indices(eff);
	    for(i=sizeof(effects); i--;)
		    effects[i]->remove_disease(tar[0]);
	}
	eff = tar[0]->query_effects()[POISON_EFFECT];
	if(!eff || !map_sizeof(eff))
	    return;
	effects = map_indices(eff);
	for(i=sizeof(effects); i--;)
	    effects[i]->remove_poison(tar[0]);
    }
}

int
query_level()
{
    return 50;
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
