inherit "../../cha_base";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Summon an animal that obeys casters order. Power manipulation " +
	"may create a stronger animal.";
}

void
spell_effect(int level, int *co, object tp)
{
    string path;
    object animal, ob, here, *cmd;
    
    if (!test_summon(co, ({ 10, 10, 10 }), I(here = E(tp), 0)))
    {
	tp->catch_tell("Something there resisted.\n");
	return;
    }
    switch (random(3))
    {
      case 0:
	path = "/lib/npc/animals/rabbit";
	break;
      case 1:
	path = "/lib/npc/animals/chicken";
	break;
      case 2:
	path = "/lib/npc/animals/rat";
	break;
    }
    animal = clone_object(path);
    animal->set_coordinates(co);
    if (ob = here->query_leaving_room(co))
	animal->move(ob);
    else
	animal->move(here);
    tell_objects(LISTEN(E(tp)),
		 ({ QCNAME(animal), " appears.\n" }), MSG_SEE, tp);
    ob = clone_object(MASTER);
    ob->store_spell(tp, level, ({ animal }));
    ob->start_call_out("remove_spell", level * 60);
    animal->add_spell(ob);
    animal->add_prop(LIVE_CONTROLLED_BY, tp);
}


void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tell_objects(LISTEN(E(tar[0])) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " disappears.\n" }), MSG_SEE,
		     tar[0]);
	remove_object(tar[0]);
    }
    destruct_object();
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
	    SP_FORCE | SP_MENTALATTACK,
	    SP_LEVEL_TIME,
	    1200,
	    SP_DIRECTION,
	    0,
	    SK_ML_PNEUMATIKOS,
	});
}
