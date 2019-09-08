inherit "/base/food";

#include <stdproperties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    add_prop(OBJ_MATERIAL, M_FOOD);
    set_name("boar meal");
    add_name("meal");
    add_adj("wild");
    set_short("boar meal");
    set_pshort("boar meals");
    set_long("This is a gourmet wild boar meal. The simple look of it makes you salivate.\n");
    set_plong("These are some gourmet wild boar meals. The simple look of them makes you salivate.\n");
    set_heap_id(MASTER);
    set_value(100);
    set_amount(600);
    add_lwvs(0, 600, 600, 7);
}
