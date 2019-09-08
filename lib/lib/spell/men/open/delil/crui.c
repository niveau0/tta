inherit "../../men_base";

#include <effects.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect if an item is cursed.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	mapping curse;

	if (!mappingp(curse = tar[0]->query_effects()[CURSE_EFFECT]) ||
	    !sizeof(map_indices(curse)))
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	tp->catch_tell(LANG_POSS(tar[0]->query_name()) seems to be cursed.\n");
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
	    SP_INSTANTLY,
	    SP_NONE,
	    SP_TOUCH,
	    1,
	    SP_ALL_OBJECTS,
	    SK_ML_CONSCIOUSNESS,
	});
}
