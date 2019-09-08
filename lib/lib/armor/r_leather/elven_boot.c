inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <race.h>
#include <properties.h>
#include <slots.h>

static void
create_armor()
{
    set_name("boot");
    add_adj("elven");
    add_adj("rigid");
    add_adj("leather");
    set_short("leather boot");
    set_pshort("leather boots");
    set_long("A boot made of rigid leather for slender feet.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_FOOT }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 2400, 1700, 16);
    set_value(150);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
