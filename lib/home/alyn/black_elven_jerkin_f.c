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
    add_adj("soft");
    add_adj("black");
    add_adj("elven");
    set_short("black soft leather jerkin");
    set_pshort("black soft leather jerkins");
    set_long_bs("This black soft leather jerkin is tailored "+
                "for female humanoids.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_CHEST }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 4000, 6000, 16);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
