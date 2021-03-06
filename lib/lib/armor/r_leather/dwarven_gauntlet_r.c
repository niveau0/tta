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
    add_adj("right");
    add_adj("rigid");
    add_adj("leather");
    add_adj("dwarven");
    set_short("gauntlet");
    set_pshort("gauntlets");
    set_long("This is a right rigid leather gauntlet made for "+
	     "small broad hands.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HAND+1 }) );
    set_race_sizes(DWARF_MIN, DWARF_MAX);
    add_lwvs(0, 270, 360, 4);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
