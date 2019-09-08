inherit "/base/seat";

#include <macros.h>
#include <material.h>
#include <properties.h>


static void
create_seat()
{
    set_name("chair");
    set_pname("chairs");
    set_short("chair");
    set_pshort("chairs");
    set_long("A simple chair.\n");
    set_lwvs(0, 1200, 9500, 5);
    set_free_seats(1);
    set_value(2);
    add_prop(OBJ_MATERIAL, M_OAK);

}
