inherit "/base/object";
//inherit "/base/burn";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("scarf");
    add_adj("silken");
    set_short("dirty scarf");
    set_pshort("dirty scarves");
    set_long("A small stripe of dirty cloth. "+
	     "Appearantly it's made of silk.\n");
    set_lwvs(0,75,20,4);
    set_value(20);
    add_prop(OBJ_MATERIAL, M_SILK);
}
