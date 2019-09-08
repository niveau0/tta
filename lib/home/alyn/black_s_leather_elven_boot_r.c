inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <race.h>
#include <properties.h>
#include <slots.h>

static void
create_armor()
{
    set_name("boot");
    add_adj("right");
    add_adj("elven");
    add_adj("soft");
    add_adj("black");
    add_adj("leather");
    set_short("black leather boot");
    set_pshort("black leather boots");
    set_long_bs("This black soft leather boot is made "+
		"for the right foot of a humanoid.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_FOOT+1 }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 2000, 1700, 16);
    set_value(130);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
