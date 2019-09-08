inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("gauntlet");
    add_adj("left");
    add_adj("rigid");
    add_adj("leather");
    add_adj("hobbit");
    set_short("gauntlet");
    set_pshort("gauntlets");
    set_long("This is a left rigid leather gauntlet made for "+
	     "small hands.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HAND+2 }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 245, 330, 4);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
