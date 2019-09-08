inherit "/base/container";
inherit "/base/misc/wear";

#include <material.h>
#include <properties.h>
#include <slots.h>

#define LONG \
"It's a fine made leather quiver to hold arrows.\n"

static void
create_container()
{
    set_name("quiver");
    add_adj("leather");
    set_short("quiver");
    set_pshort("quivers");
    set_long(LONG);
    set_lwvs(0, 1100, 323, 20);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_BACK }) );
    set_value(30);
    set_max_weight(5060);
    set_max_volume(1000);
    set_race_sizes(30, 98);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(IS_WEARABLE,1);
}
