inherit "/base/seat";

#include <macros.h>
#include <material.h>
#include <properties.h>


static void
create_seat()
{
    set_name("armchair");
    add_name("chair");
    set_pname("armchairs");
    add_pname("chairs");
    add_adj("comfortable");
    set_short("comfortable armchair");
    set_pshort("comfortable armchairs");
    set_long("This armchair is cushioned everywhere and just "+
	     "looking at it is making you feeling really comfortable.\n");
    set_lwvs(0, 4000, 15000, 27);
    set_free_seats(1);
    set_value(5);
    add_prop(OBJ_MATERIAL, M_OAK);

}
