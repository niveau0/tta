inherit "../../cha_base";

#include <effects.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Analyse if someone is poisoned.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int     i, sz;
	string  str, *ind;
	mapping poison;
	
	if (!mappingp(poison = tar[0]->query_effects()[POISON_EFFECT]) ||
	    !sizeof(ind = map_indices(poison)))
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	str = "You sense the following poisons:\n";
	for (i=0, sz=sizeof(ind); i<sz; i++)
	    str += ind[i]->query_desc() + "\n";
	tp->catch_tell(str);
    }
}

int
query_level()
{
    return 11;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_NONE,
	    600,
	    1,
	    SP_ALL_TARGETS,
	    SK_ML_CONSCIOUSNESS,
	});
}
