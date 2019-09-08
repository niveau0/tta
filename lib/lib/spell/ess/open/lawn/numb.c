inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a protecting aura against channeling magic around the " +
	"caster";
}

mixed
query_similar_code()
{
    return "_covre";
}

int
query_defense()
{
    return SK_RESIST_CHA;
}

void
do_this_effect(object t, int i)
{
    t->add_tmp_skill(query_defense(), i * 3);
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int i;
	object ob;

	i = (level-query_level()+1);
	tar[0]->catch_tell("You feel safety covering your body.\n");
	do_this_effect(tar[0], i);
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar, i);
	ob->start_concentrate(SP_TOUCH);
	tp->add_spell(ob);
	tar[0]->add_spell(ob);
    }
}

void
remove_spell()
{
    object c;
    
    if (c = query_caster())
    {
	c->catch_tell("You feel that some safety is gone.\n");
	do_this_effect(c, query_data() * (-1));
	c->delete_state(CONCENTRATE);
	c->add_prop(CONCENTRATE_DATA, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 12;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_CONCENTRATE,
	SP_SELF,
	1,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
