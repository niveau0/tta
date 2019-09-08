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
    add_adj("soft");
    add_adj("small");
    add_adj("leather");
    set_short("small soft leather helmet");
    set_pshort("small soft leather helmets");
    set_long("A small simple helmet, made of soft leather.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({SLT_HEAD}) );
    set_race_sizes(HOBBIT_MIN, DWARF_MAX);
    add_lwvs(0, 950, 3700, 12);
    set_value(145);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
