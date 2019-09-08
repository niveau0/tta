inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("chainshirt");
    add_adj("elven");
    add_name("shirt");
    set_short("chainshirt");
    set_pshort("chainshirts");
    set_long("A cainmail shirt that covers the abdomen. "+
	     "It is made for humanoids.\n");
    set_at(A_CHAIN);
    set_layer(LAYER_CHAIN);
    set_locations( ({ SLT_CHEST, SLT_ABDOMEN }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 9750, 10000, 10);
    set_value(1500);
    add_prop(OBJ_MATERIAL, M_STEEL);
}
