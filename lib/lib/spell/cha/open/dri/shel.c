inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <state.h>

string
query_desc()
{
    return "Protection shield against all realms.";
}

mixed
query_similar_code()
{
    return "_shel";
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    tp->catch_tell("You protect yourself.\n");
    ob = clone_object(MASTER);
    ob->store_spell(tp, level, tar);
    ob->start_concentrate(SP_SELF);
    tp->add_spell(ob);
    tp->cb_add_boni(({ CB_RESIST_CHA, CB_RESIST_ESS, CB_RESIST_MEN }),
		    ({ ([ ob : 10 ]), ([ ob : 10 ]), ([ ob : 10 ]) }));
}

void
remove_spell()
{
    object c;

    if (c = query_caster())
    {
	c->delete_state(CONCENTRATE);
	c->add_prop(CONCENTRATE_DATA, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 19;
}

int *
query_spell_data()
{
    return
	({
	    SP_DEFENSIVE,
	    SP_CONCENTRATE,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_PETRA,
	});
}
