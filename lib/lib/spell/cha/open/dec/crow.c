inherit "../../cha_base";

#include <effects.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect if someone/something is cursed.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	string  *ind;
	mapping curse;

	if (!mappingp(curse = tar[0]->query_effects()[CURSE_EFFECT]) ||
	    !sizeof(ind = map_indices(curse)))
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	tp->catch_tell("You sense " + sizeof(ind) + " curses.\n");
    }
}

int
query_level()
{
    return 5;
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
