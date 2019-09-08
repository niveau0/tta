inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("pants");
    add_name("pair of trousers");
    add_name("trousers");
    add_adj("linen");
    add_adj("hobbit");
    add_adj("yellow");
    set_short("linen pants");
    set_pshort("linen pants");
    set_long_bs("These yellow linen pants are probably made for small " +
                "humanoids.\n");
    set_at(A_SKIN);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_ABDOMEN, SLT_LEG+1, SLT_LEG+2 }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 2500, 7800, 8);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LINEN);
}
