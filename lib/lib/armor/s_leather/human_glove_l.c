inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("glove");
    add_adj("left");
    add_adj("soft");
    add_adj("leather");
    add_adj("human");
    set_short("glove");
    set_pshort("gloves");
    set_long("This is a left hand soft leather glove.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HAND+2 }) );
    set_race_sizes(HUMAN_MIN, HUMAN_MAX);
    add_lwvs(0, 300, 400, 5);
    set_value(65);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
