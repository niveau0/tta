#include <material.h>
#include <properties.h>

inherit "/base/seat";

static void
create_seat()
{
    set_name("bench");
    set_pname("benches");
    set_short("bench");
    set_pshort("benches");
    set_long("A long wooden bench.\n");
    set_lwvs(0, 50000, 35000, 40);
    set_free_seats(5);
    set_value(50);
    add_prop(OBJ_MATERIAL, M_OAK);
}
