inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "A very powerful healing spell. All concussion wounds will be " +
	"healed at once if the casting is successful. Power manipulates the " +
	"possible number of targets.";
}

void
message(object tp, object *tar)
{
    tp->catch_tell("You just invoke a strong healing aura.\n");
}

void
spell_effect(int level, object *tar, object tp)
{
    int i, sz;

    if (sz = sizeof(tar))
    {
	tell_objects(tar, "Vitality flows through your body.\n", 0);
	for (i=0, sz=sizeof(tar); i<sz; i++)
	    tar[i]->set_hp(tar[i]->query_max_hp());
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
	    SP_PERMANENT,
	    1200,
	    SP_LEVEL_TARGETS,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
