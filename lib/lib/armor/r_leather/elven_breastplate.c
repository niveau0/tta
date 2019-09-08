inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("breastplate");
    add_name("plate");
    add_adj("elven");
    set_short("breastplate");
    set_pshort("breastplates");
    set_long("A breastplate of rigid leather made for "+
	     "slender humanoids.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_CHEST }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 6400, 12000, 17);
    set_value(450);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
