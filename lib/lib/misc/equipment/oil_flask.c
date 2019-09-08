inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("flask");
    add_adj("oil");
    set_short("oil flask");
    set_pshort("oil flasks");
    set_long("It's a glass flask.\n");
    set_lwvs(0, 200, 50,7);
    set_max_volume(50 + 500);
    set_max_weight(200 + 500);
    set_fill(500);
    set_liquid_type(LIQ_OIL);
    set_value(40);
    add_prop(OBJ_MATERIAL, M_GLASS);
    add_prop(CONT_RIGID, 1);
}
