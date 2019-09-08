inherit "../../cha_base";

#include <clone.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <properties.h>
#include <skills.h>
#include <state.h>

string
query_desc()
{
    return "Create a foggy cloud which slows down movement. The cloud " +
	"stays around " + (TIME_SCALE * query_level()) +
	" minutes, but power manipulation could increase this duration.";
}

void
spell_effect(int level, int *co, object tp)
{
    object wall, ob, here;

    wall = clone_object(CLONE_AIRWALL);
    wall->set_range(({ 120, 120, 120 }));
    wall->set_coordinates(co);
    here = E(tp);
    if (ob = here->query_leaving_room(co))
	wall->move(ob);
    else
	wall->move(here);
    ob = clone_object(MASTER);
    ob->store_spell(tp, level, ({ wall }));
    ob->start_call_out("remove_spell", level * 60);
    wall->add_spell(ob);
    tell_objects(LISTEN(E(tp)),
		 ({ QCNAME(wall), " appears.\n" }), MSG_SEE, tp);
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
    return 11;
}

int *
query_spell_data()
{
    return
	({
	    SP_ELEMENTAL,
	    SP_LEVEL_TIME,
	    600,
	    SP_DIRECTION,
	    0,
	    SK_ML_PETRA,
	});
}
