inherit "../../cha_base";

#include <clone.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Summon a permanent earthwall. You can control the orientation " +
	"if you specify 'ns' for north to south or 'ew' for east to west in " +
	"your spell. More power makes it as always more difficult to dispel.";
}

void
spell_effect(int level, int *co, object tp, string orientation)
{
    int    *range;
    object wall, ob, here;

    if (orientation == "ns" || orientation == "sn")
	range = ({ 36, 120, 120 });
    else
	range = ({ 120, 36, 120 });
    
    if (!test_summon(co, range, I(here = E(tp), 0)))
    {
	tp->catch_tell("Something there resisted.\n");
	return;
    }
    wall = clone_object(CLONE_EARTHWALL);
    wall->set_hp(level * 60, level * 60);
    wall->set_range(range);
    wall->set_coordinates(co);
    if (ob = here->query_leaving_room(co))
	wall->move(ob);
    else
	wall->move(here);
    tell_objects(LISTEN(E(tp)),
		 ({ QCNAME(wall), " appears.\n" }), MSG_SEE, tp);
    ob = clone_object(MASTER);
    ob->store_spell(tp, level, ({ wall }));
    wall->add_spell(ob);
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tell_objects(LISTEN(E(tar[0])) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " molders to dust.\n" }), MSG_SEE,
		     tar[0]);
	remove_object(tar[0]);
    }
    destruct_object();
}

int
query_level()
{
    return 17;
}

int *
query_spell_data()
{
    return
	({
	    SP_ELEMENTAL,
	    SP_PERMANENT,
	    600,
	    SP_DIRECTION,
	    0,
	    SK_ML_PETRA,
	});
}
