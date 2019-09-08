inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("bracer");
    add_adj("rigid");
    add_adj("leather");
    set_short("bracer");
    set_pshort("bracers");
    set_long("This is a rigid leather bracer.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_ARM }) ); /* fits right and left arm */
    set_race_sizes(DWARF_MIN, ELF_MAX);
    add_lwvs(0, 700, 550, 5);
    set_value(150);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
