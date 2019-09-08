inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("greaves");
    add_adj("rigid");
    add_adj("leather");
    add_adj("leg");
    add_adj("elven");
    set_short("leg greaves");
    set_pshort("leg greaves");
    set_long("This is a rigid leather leg protection made for "+
	     "slender legs.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_LEG }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 2300, 3000, 15);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
