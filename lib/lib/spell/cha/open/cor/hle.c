inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "A healing spell for concussion wounds. The spell cannot heal " +
	"fractures or bloodloss. Power manipulates increases the " +
	"healing power.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	tar[0]->catch_tell("You feel vitality flowing through your body.\n");
	if (level > 30)
	    tar[0]->add_hp(tar[0]->query_max_hp());
	else
	    tar[0]->add_hp(random(level/3 * 20) + level);
    }
}

int
query_level()
{
    return 2;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_PERMANENT,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
