inherit "/base/object";
inherit "/base/misc/wear";

#include <event.h>
#include <living.h>
#include <material.h>
#include <macros.h>
#include <properties.h>
#include <slots.h>

static nomask void
create_object()
{
    set_name("amulet");
    set_pname("amulets");
    add_adj("small");
    set_short("small amulet");
    set_pshort("small amulets");
    set_long("A small fine worked amulet with a green stone in it.\n");
    set_lwvs(0, 5, 3, 2);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_NECK }) );
    set_value(300000);
    set_race_sizes(0, 350);
    add_prop(OBJ_MATERIAL, M_ENCH_IRON);
    add_prop(IS_WEARABLE,1);
    add_event(E_WEAR, "object_worn", TO());
    add_event(E_REMOVE_TOOL, "object_removed", TO());
}

nomask string
wear_preposition()
{
    return "around";
}

nomask void
object_worn(object tp, object ob)
{
    if (!tp || ob != TO())
	return;
    add_event(E_DEATH, "resurrect", tp);
}

nomask void
object_removed(object tp, object ob)
{
    if (!tp || ob != TO())
	return;
    remove_event(E_DEATH, "resurrect", tp);
}

nomask void
resurrect(object killed, object killer)
{
    if (!killed || query_wearer() != killed)
	return;
    
    killed->resurrect();
    killed->catch_tell("When you hear your last heartbeat and you know "+
		       "your life ended, your worn\namulet suddenly glows "+
		       "with a bright light and is gone.\nYou feel "+
		       "vital and relaxed again.\n");
    if (killer && killer != killed)
	killer->catch_tell("You just killed your enemy - you thought. But "+
			   "suddenly "+killed->query_possessive()+" amulet "+
			   "begins\nto glow and is gone. After that "+
			   killed->query_pronoun()+
			   " seems to be fully healed again.\n");
    tell_objects(LISTEN(E(killed)) - ({ killed , killer}),
		 ({ QCTNAME(killed), " seems to be killed, but suddenly "+
			killed->query_possessive()+" amulet begins to glow "+
			"- then evaporates and "+killed->query_pronoun()+
			" is fully healed again.\n" }), MSG_SEE);	 
    killed->set_hp(killed->query_max_hp());
    killed->set_fatigue(killed->query_max_fatigue());
    remove_object();
}
