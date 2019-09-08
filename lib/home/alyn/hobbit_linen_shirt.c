inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("shirt");
    add_adj("linen");
    add_adj("hobbit");
    add_adj("white");
    set_short("cotton shirt");
    set_pshort("cotton shirt");
    set_long_bs("This white linen shirt has two long sleeves and is probably " +
		"made for small humanoids.\n");
    set_at(A_SKIN);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_CHEST, SLT_ARM+1, SLT_ARM+2 }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 2400, 7800, 8);
    set_value(50);
    add_prop(OBJ_MATERIAL, M_LINEN);
}
