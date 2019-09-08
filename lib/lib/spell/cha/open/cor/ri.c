inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <state.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "A regeneration spell for concussion wounds. The spell " +
	"cannot heal fractures or bloodloss, but heals concussion damage " +
	"as long as the caster concentrates. Power manipulates " +
	"healing rate.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	if (tar[0] == tp)
	    tp->catch_tell("You start to regenerate.\n");
	else
	    tar[0]->catch_tell("Vitality flows through your body.\n");

	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_concentrate(SP_TOUCH);
	tp->add_spell(ob);
	if (tp != tar[0])
	    tar[0]->add_spell(ob);
    }
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

static void
concentrate(mixed arg)
{
    if (sizeof(query_targets()))
	query_targets()[0]->add_hp(query_cast_level() / 7);
    ::concentrate(arg);
}

int
query_level()
{
    return 7;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_CONCENTRATE,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
