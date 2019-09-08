inherit "/base/container";

#include <material.h>
#include <properties.h>
#include <slots.h>

static void
create_container()
{
    set_name("bag");
    add_adj("large");
    set_short("large bag");
    set_pshort("large bags");
    set_long("A bag made of jute and it looks as if it could carry " +
	     "a whole chicken.\n");
    set_lwvs(0,50,25,12);
    set_value(7);
    set_max_weight(10000);
    set_max_volume(9000);
    add_prop(OBJ_MATERIAL, M_LINEN);
}
