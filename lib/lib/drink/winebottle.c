inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("bottle");
    add_adj("glass");
    add_adj("brown");
    set_short("glass bottle");
    set_pshort("glass bottles");
    set_long_bs("The bottle is made from brown glass that is transparent "+
	     "enough to see inside.\n");
    set_lwvs(0, 450, 300, 13);
    set_max_weight(400 + 2000);
    set_max_volume(300 + 1000);
    set_liquid_type(LIQ_WINE, "clear yellow", "wine");
    set_fill(990);
    set_value(13);
    add_prop(OBJ_MATERIAL, M_GLASS);
    add_prop(CONT_CLOSED, 1);
    add_prop(CONT_TRANSP, 1);
    add_prop(CONT_RIGID, 1);
}
