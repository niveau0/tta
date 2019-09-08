inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("cup");
    add_adj("small");
    set_short("small cup");
    set_pshort("small cups");
    set_long("A small wooden cup.\n");
    set_max_weight(200 + 250);
    set_max_volume(150 + 250);
    set_lwvs(0, 200, 150, 10);
    set_liquid_type(LIQ_WATER);
    set_fill(250);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_TRANSP, 0);
}
