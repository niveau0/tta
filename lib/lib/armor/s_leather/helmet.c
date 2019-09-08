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
    add_adj("leather");
    set_short("soft leather helmet");
    set_pshort("soft leather helmets");
    set_long("A simple helmet, made of soft leather.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({SLT_HEAD}) );
    set_race_sizes(DWARF_MIN, ELF_MAX);
    add_lwvs(0, 1050, 5000, 14);
    set_value(150);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
