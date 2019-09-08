inherit "/base/container";
inherit "/base/misc/wear";

#include <properties.h>
#include <material.h>
#include <slots.h>

#define LONG \
"It's a fine made leather backpack which helps to carry things easier.\n"

static void
create_container()
{
    set_name("backpack");
    add_adj("large");
    add_adj("leather");
    set_short("large backpack");
    set_pshort("large backpacks");
    set_long(LONG);
    set_lwvs(0, 1533, 323, 30);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_BACK }) );
    set_value(22);
    set_max_weight(9060);
    set_max_volume(10000);
    set_race_sizes(60, 98);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(IS_WEARABLE,1);
}
