inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <stdproperties.h>

static void
create_container()
{
    set_name("bucket");
    add_adj("wooden");
    add_adj("sturdy");
    set_short("wooden bucket");
    set_pshort("wooden buckets");
    set_long("A bucket which has been made from some kind of wood."+
	     " Though seemingly not crafted by a highskilled living "+
	     "it is quite sturdy. ");
    /* Sind die Werte vernuenftig?
     */
    set_max_weight(3000 + 25000);
    set_max_volume(12000);
    set_lwvs(0, 3000, 4000, 20);
    set_value(7);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(CONT_RIGID, 1);
}
