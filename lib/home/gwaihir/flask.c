inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("flask");
    add_adj("small");
    set_short("small flask");
    set_pshort("small flasks");
    set_long("A small glass flask.\n");
    set_max_weight(200 + 1500);
    set_max_volume(150 + 1500);
    set_lwvs(0, 200, 150, 10);
    set_liquid_type(LIQ_WATER);
    set_fill(1500);
    add_prop(OBJ_MATERIAL, M_GLASS);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_TRANSP, 1);
}
