inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("helmet");
    add_adj("plate");
    set_short("plate helmet");
    set_pshort("plate helmets");
    set_long("A fine made helmet of good steel.\n");
    set_at(A_PLATE);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HEAD }) );
    set_race_sizes(DWARF_MIN, ELF_MAX);
    add_lwvs(0, 1450, 5000, 14);
    set_value(400);
    add_prop(OBJ_MATERIAL, M_STEEL);
}
