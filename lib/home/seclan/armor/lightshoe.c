inherit "/base/armor";

#include <material.h>
#include <combat.h>
#include <slots.h>
#include <properties.h>

static void
create_armor()
{
    set_name("shoe");
    add_adj("amorph");
    add_adj("glowining");
    set_short("glowing shoe");
    set_pshort("glowing shoes");
    set_long_bs("This shoe is made from a slighly glowing material. "+
	     "The amorph foil seems to be elastic, but it always keeps "+
	     "the shape of a foot.");
    set_at(A_S_LEATHER);
    set_layer(LAYER_ABOVE_ALL);
    set_locations( ({ SLT_FOOT }) );
    add_lwvs(1, 0, 0, 4);
    set_race_sizes(0, 500);
    set_value(800);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_THIN_MATERIAL, 1);
}

