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
    add_adj("medium");
    set_short("medium plate helmet");
    set_pshort("medium plate helmets");
    set_long("A medium sized helmet of good steel.\n");
    set_at(A_PLATE);
    set_layer(LAYER_PLATE);
    set_locations( ({SLT_HEAD}) );
    set_race_sizes(DWARF_MIN, ELF_MAX);
    add_lwvs(0, 1375, 3700, 12);
    set_value(450);
    add_prop(OBJ_MATERIAL, M_STEEL);
}
