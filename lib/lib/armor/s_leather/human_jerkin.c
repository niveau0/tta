inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("jerkin");
    add_adj("leather");
    add_adj("human");
    set_short("leather jerkin");
    set_pshort("leather jerkins");
    set_long("A simple leather jerkin.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_CHEST }) );
    set_race_sizes(HUMAN_MIN, HUMAN_MAX);
    add_lwvs(0, 3600, 5800, 15);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
