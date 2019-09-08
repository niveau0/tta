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
    add_adj("dwarven");
    set_short("breastplate");
    set_pshort("breastplates");
    set_long("A breastplate of rigid leather for small humanoids.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_CHEST }) );
    set_race_sizes(DWARF_MIN, DWARF_MAX);
    add_lwvs(0, 5750, 9000, 13);
    set_value(450);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
