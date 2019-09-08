inherit "/base/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("white bread");
    set_name("bread");
    set_short("white bread");
    set_pshort("white breads");
    set_long("This seems to be some nourishing white bread.\n");
    set_plong("These seem to be some nourishing white breads.\n");
    set_heap_id(MASTER);
    set_value(20);
    set_amount(500); /* gram */
    add_lwvs(0, 510, 400, 5);
    add_prop(OBJ_MATERIAL, M_FOOD);
}
