inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container() /* Weight, Volume and so on has to be considered
		    * more closely.
		    */
{
    set_name("keg");
    add_name("beerkeg");
    add_name("barrel");
    add_adj("small");
    add_adj("wooden");
    set_short("wooden keg");
    set_pshort("wooden kegs");
    set_long_bs("It is a keg, made from wood and designed to transport "+
	     "liquids. The smell it evaporates indicates that beer was "+
	     "in closer contact to it.\n");
    set_lwvs(0, 1500, 2500, 18);
    set_max_weight(1500 + 18000);
    set_max_volume(2500 + 11000);
    set_liquid_type(LIQ_BEER, "light yellow", "beer");
    set_fill(10800);
    set_value(12);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(CONT_CLOSED, 1);
    add_prop(CONT_RIGID, 1);
}
