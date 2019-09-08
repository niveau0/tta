inherit "/base/container";

#include <material.h>
#include <properties.h>
#include <slots.h>

static void
create_container()
{
    set_name("sack");
    add_adj("large");
    set_short("large sack");
    set_pshort("large sacks");
    set_long("This is a dirty old sack, normally used by millers.\n");
    set_lwvs(0,1300,400,20);
    set_value(8);
    set_max_weight(25000);
    set_max_volume(30000);
    add_prop(OBJ_MATERIAL, M_LINEN);
}
