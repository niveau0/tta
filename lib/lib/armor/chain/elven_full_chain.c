inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("fullchain");
    add_adj("elven");
    add_name("chain");
    set_short("fullchain");
    set_pshort("fullchains");
    set_long("A long sleeved chainmail shirt and chainmail leggins. "+
	     "It is made for humanoids.\n");
    set_at(A_CHAIN);
    set_layer(LAYER_CHAIN);
    set_locations( ({ SLT_CHEST, SLT_ABDOMEN, SLT_LEG+1, SLT_LEG+2,
			  SLT_ARM+1, SLT_ARM+2 }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 21550, 15000, 15);
    set_value(6500);
    add_prop(OBJ_MATERIAL, M_STEEL);
}
