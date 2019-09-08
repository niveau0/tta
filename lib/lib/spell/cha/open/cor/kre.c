inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "A stun relief spell. If you are stunned, this spell helps. " +
	"Power manipulation heals better if you are badly damaged.";
}

void
spell_effect(int level, object *tar, object tp)
{
    int     i, sz, count, *id;
    mapping map;
    mixed   ind;
    
    if (!sizeof(tar))
	return;

    tar[0]->catch_tell("You feel magic floating through your body.\n");
    tar[0]->cb_query_bonus(CB_STUNNED); /* update bonus map */
    if (!mappingp(map = tar[0]->cb_bonus_map()[CB_STUNNED]))
	return;
    ind = map_indices(map);
    count = level / 10 + 1;
    id = ({ CB_STUNNED });
    
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if (count-- <= 0)
	    break;
	tar[0]->cb_remove_boni( id, ({ ind[i] }) );
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
	    (SP_HEALING | SP_INSTANTLY),
	    SP_PERMANENT,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
