inherit "../../cha_base";

#include <effects.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Heal someone suffering from a poison.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	string  *files;
	mapping poison;
	
	tar[0]->catch_tell("A cleaning feeling flows through your body.\n");
	
	if (!mappingp(poison = tar[0]->query_effects()[POISON_EFFECT]))
	    return;
	if (!sizeof(files = map_indices(poison)))
	    return;
	files[0]->remove_poison(tar[0]);
    }
}

int
query_level()
{
    return 15;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_PERMANENT,
	    120,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
