inherit "/base/armor";

#include <material.h>
#include <combat.h>
#include <slots.h>
#include <properties.h>


static void
create_armor()
{
    set_name("cape");
    add_adj("amorph");
    add_adj("shimmering");
    set_short("shimmering cape");
    set_pshort("shimmering capes");
    set_long_bs("A glowing and amorph foil that is vaguely formed like a "+
	     "hooded cape of uncertain size. ");
    set_at(A_S_LEATHER);
    set_layer(LAYER_CAPE);
    set_locations( ({ SLT_HEAD+1, SLT_EAR+1, SLT_EAR+2, SLT_BACK+1,
			  SLT_LEG+1, SLT_LEG+2, SLT_NECK+1 }) );
    add_lwvs(1, 0, 1, 70);
    set_race_sizes(0, 500);
    set_value(1000);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_THIN_MATERIAL, 1);
}

