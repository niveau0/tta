inherit "/base/container";
inherit "/base/misc/wear";

#include <event.h>
#include <material.h>
#include <properties.h>
#include <slots.h>
#include <macros.h>
#include <skills.h>

static void
create_container()
{
    object ob;
    
    set_name("scabbard");
    add_adj("leather");
    add_adj("black");
    set_short("scabbard");
    set_pshort("scabbards");
    set_long_bs("This black leather scabbard is made to hold a shortsword.\n");
    set_lwvs(0, 400, 230, 20);
    set_layer(LAYER_ABOVE_ALL);
    set_value(30);
    set_max_weight(5060);
    set_max_volume(1000);
    set_race_sizes(30, 98);
    set_locations( ({ SLT_ABDOMEN }) );
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(OBJ_ATTACH_SIZE, 2);
    add_event(E_MOVE_ATTEMPT, "sheathing", this_object());
}

int sheathing(object ob, object env_old, object env_new)
{
    debug("Alyn", ob);
    return 1;
}
    



















































