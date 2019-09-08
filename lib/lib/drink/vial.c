inherit "/base/container";

#include <material.h>
#include <liquid.h>
#include <properties.h>

static void
create_container()
{
    int blubber;

    set_name("vial");
    add_adj("glass");
    set_short("glass vial");
    set_pshort("glass vials");
    set_long("It's a small fragile glass vial.\n");
    set_lwvs(0,75,50,4);
    set_value(20);
    set_max_weight(75 + 125);
    set_max_volume(50 + 125);
    add_prop(OBJ_MATERIAL, M_GLASS);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_TRANSP, 1);

    set_liquid_type(LIQ_QUILMUFUR, "green", "quilmufur");
    set_fill(125);
}
