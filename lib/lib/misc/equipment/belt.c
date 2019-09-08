inherit "/base/object";
inherit "/system/inventory";
inherit "/base/misc/wear";
inherit "/base/misc/attach";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <slots.h>

static nomask void
create_object()
{
    set_name("belt");
    set_pname("belts");
    add_adj("leather");
    set_short("leather belt");
    set_pshort("leather belts");
    set_long("A thick leather belt with four nooses to hold items.\n");
    set_lwvs(0, 500, 323, 10);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_ABDOMEN }) );
    set_value(30);
    set_race_sizes(60, 90);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(IS_WEARABLE, 1);
    
    set_attach_slots(4);
}
