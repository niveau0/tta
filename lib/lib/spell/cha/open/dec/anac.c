inherit "../../cha_base";

#include <effects.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "If someone is cursed, analyse the curse.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int     i, sz;
	string  str, *ind;
	mapping curse;
	
	if (!mappingp(curse = tar[0]->query_effects()[CURSE_EFFECT]) ||
	    !sizeof(ind = map_indices(curse)))
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	str = "You sense the following curses:\n";
	for (i=0, sz=sizeof(ind); i<sz; i++)
	    str += ind[i]->query_desc() + "\n";
	tp->catch_tell(str);
    }
}

int
query_level()
{
    return 20;
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
