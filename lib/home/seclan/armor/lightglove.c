inherit "/base/armor";

#include <material.h>
#include <combat.h>
#include <slots.h>
#include <properties.h>

static void
create_armor()
{
    set_name("glove");
    add_adj("soft");
    add_adj("glistening");
    set_short("glistening glove");
    set_pshort("glistening gloves");
    set_long_bs("This glistening glove is "+
	     "made from a thin foil that clinges to the body. "+
	     "You fail to think of the hand it might cover due its "+
	     "elastic capabilities.");
    set_at(A_S_LEATHER);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_HAND }) );
    add_lwvs(1, 0, 0, 2);
    set_race_sizes(0, 500);
    set_value(800);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_THIN_MATERIAL, 1);
}

