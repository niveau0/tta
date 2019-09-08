inherit "/base/armor";

#include <material.h>
#include <combat.h>
#include <slots.h>
#include <stdproperties.h>

static void

create_armor()
{
    set_name("mask");
    add_adj("iridescent");
    set_short("iridescent mask");
    set_pshort("iridescent masks");
    set_long("An oval piece of clothing with seemingly no weight. "+
	     "While one side of it only presents an iridescent glisten "+
	     "the other side allows to look and breathe through.\n");
    set_aat(ATT_S_LEATHER, 0, 50);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_HEAD+1 }) );
    add_lwvs(1, 0, 0, 5);
    set_race_sizes(0, 500);
    set_value(1500);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_THIN_MATERIAL, 1);
}

void
set_wearer(object w)
{
    ::set_wearer(w);

}
