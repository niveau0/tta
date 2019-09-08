inherit "/base/object";
inherit "../base/seat";

#include <macros.h>
#include <properties.h>
#include <material.h>


create_seat()
{
    set_name("pillow");
    add_adj("silver");
    set_pname("pillows");
    set_short("silver pillow");
    set_pshort("silver pillows");
    set_long_bs("An over-sized, fluffy silver pillow.\n");
    set_lwvs(0, 75000, 50000, 25);
    set_free_seats(1);
    set_value(0);
    add_prop(OBJ_MATERIAL, M_SILK);
    add_prop(OBJ_NO_GET, 1);
}
