inherit "../../cha_base";

#include <liquid.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Create water. You need a receptacle for the water.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	if (tar[0]->query_prop(IS_CONT))
	{
	    tp->catch_tell("You need a receptacle.\n");
	    return;
	}
	if (tar[0]->add_fill(level * 1000) > 0)
	    tp->catch_tell("You just created some water.\n");
	else
	    tp->catch_tell("It seems to be already full.\n");
	if (!tar[0]->query_liquid_type())
	    tar[0]->set_liquid_type(LIQ_WATER);
    }
}

int
query_level()
{
    return 3;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_PERMANENT,
	    120,
	    1,
	    SP_ALL_OBJECTS,
	    SK_ML_QADOSH,
	});
}
