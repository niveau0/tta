inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Close a bleeding wound. This does not heal the damage received " +
	"from the bloodloss.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int     *ind;
	mapping wounds;

	if (!map_sizeof(wounds = tar[0]->query_bloodwounds()))
	{
	    if (tar[0] == tp)
		tp->catch_tell("You are not bleeding.\n");
	    else
		tp->catch_tell(capitalize(tar[0]->query_pronoun()) +
			       " is not bleeding.\n");
	    return;
	}
	ind = map_indices(wounds);
	if (level > 17)
	    tar[0]->add_bloodloss(-wounds[ind[0]], ind[0]);
	else
	    tar[0]->add_bloodloss(-level / 6, ind[0]);
    }
}

int
query_level()
{
    return 6;
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
