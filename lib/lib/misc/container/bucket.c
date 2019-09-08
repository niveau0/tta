inherit "/base/container";

#include <properties.h>
#include <material.h>
#include <slots.h>

static void
create_container()
{
    set_name("bucket");
    add_adj("copper");
    set_short("bucket");
    set_pshort("buckets");
    set_long("This is an old copper bucket.\n");
    set_lwvs(0,1500,400,14);
    set_value(40);
    set_max_weight(16000);
    set_max_volume(14000);
    add_prop(CONT_RIGID, 1);
    add_prop(OBJ_MATERIAL, M_COPPER);
}
