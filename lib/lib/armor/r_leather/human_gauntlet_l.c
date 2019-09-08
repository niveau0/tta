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
    add_adj("human");
    set_short("gauntlet");
    set_pshort("gauntlets");
    set_long("This is a left rigid leather gauntlet.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HAND+2 }) );
    set_race_sizes(HUMAN_MIN, HUMAN_MAX);
    add_lwvs(0, 300, 400, 5);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
