inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Detect if someone/something is magically enchanted.";
}

int
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	tp->catch_msg(({ QCTNAME(tar[0]), " is " +
			     ((tar[0]->query_prop(OBJ_MAGIC) > 0 ||
			       sizeof(tar[0]->query_spells()))
			      ? "" : "not") + " magically enchanted.\n" }),
		      tar[0], MSG_SEE);
    }
    return 1;
}

int
query_level()
{
    return 10;
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
