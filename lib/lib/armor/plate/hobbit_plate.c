inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("platemail");
    add_name("platearmor");
    add_name("plate");
    set_short("platearmor");
    set_pshort("platearmors");
    set_long("A fine platearmor made of good steel for small humanoids.\n");
    set_at(A_PLATE);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_CHEST, SLT_ABDOMEN }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 8750, 15000, 25);
    set_value(20000);
    add_prop(OBJ_MATERIAL, M_STEEL);
}
