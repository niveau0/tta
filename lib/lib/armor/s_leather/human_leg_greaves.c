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
    add_adj("soft");
    add_adj("leather");
    add_adj("leg");
    add_adj("human");
    set_short("leg greaves");
    set_pshort("leg greaves");
    set_long("This is a soft leather leg protection.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_LEG }) );
    set_race_sizes(HUMAN_MIN, HUMAN_MAX);
    add_lwvs(0, 2000, 2345, 13);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
