inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "Summon food. Creates a piece of bread.";
}

void
spell_effect(int level, object *tar, object tp)
{
    int    i, sz, vol, *hands;
    object ob;

    ob = clone_object("/lib/food/bread_500g");
    ob->set_heap_size(level / 3);
    vol = ob->query_volume();
    tp->catch_tell("You just created some food.\n");
    hands = tp->query_slot_type(SLT_HAND);
    for (i=0, sz=sizeof(hands); i<sz; i++)
	if (tp->slot_space(hands[i], 0) >= vol)
	{
	    tp->add_tool(ob, hands[i]);
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ "Suddenly some bread appears in ", QTNAME(tp),
			    "'s hands.\n" }), MSG_SEE, tp);
	    return;
	}

    ob->set_coordinates(tp->query_coordinates());
    ob->move(E(tp));
    tp->catch_tell("You have no hands free so it falls on the ground.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ "Suddenly some bread appears before ", QTNAME(tp),
		    ".\n" }), MSG_SEE, tp);
}

int
query_level()
{
    return 4;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_PERMANENT,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_QADOSH,
	});
}
