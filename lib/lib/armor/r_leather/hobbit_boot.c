inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("boot");
    add_adj("hobbit");
    add_adj("rigid");
    add_adj("leather");
    set_short("leather boot");
    set_pshort("leather boots");
    set_long("A boot made of rigid leather for small feet.\n");
    set_at(A_R_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_FOOT }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 2000, 1500, 14);
    set_value(150);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
