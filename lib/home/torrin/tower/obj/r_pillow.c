inherit "/base/object";
inherit "../base/seat";

#include <macros.h>
#include <properties.h>
#include <material.h>

static void
create_seat()
{
    set_name("pillow");
    add_adj("red");
    set_pname("pillows");
    set_short("red pillow");
    set_pshort("red pillows");
    set_long_bs("An over-sized, fluffy red pillow.\n");
    set_lwvs(0, 75000, 50000, 25);
    set_free_seats(1);
    set_value(0);
    add_prop(OBJ_MATERIAL, M_SILK);
    add_prop(OBJ_NO_GET, 1);
}
