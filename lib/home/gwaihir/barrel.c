inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("barrel");
    add_adj("big");
    add_adj("wooden");
    set_short("big barrel");
    set_pshort("big barrels");
    set_long("A big wooden barrel.\n");
    set_max_weight(20000 + 250000);
    set_max_volume(2500 + 250000);
    set_lwvs(0, 2000, 1500, 100);
    set_liquid_type(LIQ_WATER);
    set_fill(250000);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_TRANSP, 0);
    
}
