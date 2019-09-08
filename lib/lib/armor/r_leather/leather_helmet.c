inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <race.h>
#include <slots.h>
#include <properties.h>

static void
create_armor()
{
    set_name("helmet");
    add_adj("rigid");
    add_adj("leather");
    set_short("rigid leather helmet");
    set_pshort("rigid leather helmets");
    set_long("A simple helmet, made of rigid leather.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HEAD }) );
    set_race_sizes(DWARF_MIN, ELF_MAX);
    add_lwvs(0, 1050, 5000, 14);
    set_value(250);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
