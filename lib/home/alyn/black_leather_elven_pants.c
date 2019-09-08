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
    add_adj("leather");
    add_adj("elven");
    add_adj("black");
    set_short("leather pants");
    set_pshort("leather pants");
    set_long_bs("These black leather pants are probably made for humanoids.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_ABDOMEN, SLT_LEG+1, SLT_LEG+2 }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 4000, 8000, 8);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
