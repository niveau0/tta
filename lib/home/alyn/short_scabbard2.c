inherit "/base/container";
inherit "/base/misc/wear";

#include <event.h>
#include <material.h>
#include <properties.h>
#include <slots.h>
#include <skills.h>
#include <language.h>
#include <combat.h>
#include <macros.h>

static void
create_container()
{    
    set_name("scabbard");
    add_adj("leather");
    add_adj("black");
    set_short("scabbard");
    set_pshort("scabbards");
    set_long_bs("This black leather scabbard is made to hold a shortsword. " +
		"You can wear it on the back.\n");
    set_lwvs(0, 400, 230, 12);
    set_layer(LAYER_ABOVE_ALL);
    set_value(30);
    set_max_weight(5060);
    set_max_volume(1000);
    set_race_sizes(30, 98);
    set_locations( ({ SLT_BACK }) );
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(IS_WEARABLE,1);
    add_event(E_MOVE_ATTEMPT, "sheathing", this_object());
}

int sheathing(object ob, object env_old, object env_new)
{
    int max_shortsword_size;
    
    max_shortsword_size=35;
    
    if (ob == this_object() || env_old == this_object())
	return 0;
    
    if (ob->query_size()<max_shortsword_size)
    {
	return 0;
    }
    else
    {	
	if (living(E(ob)))
	{
	    E(ob)->catch_tell(capitalize(LANG_ADDART(ob->query_name())) +
			      " is too long for this scabbard.\n");
	}
    }
    return 1;
}
