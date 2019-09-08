inherit "/base/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("steak");
    set_name("t-bone steak");
    set_short("bloody steak");
    set_pshort("bloody steaks");
    set_long("This is a fresh t-bone steak. You think you should roast it!\n");
    set_plong("These are some fresh t-bone steaks. You think you should "+
	      "roast them!\n");
    set_heap_id(MASTER);
    set_value(125);
    set_amount(400);
    add_prop(OBJ_MATERIAL, M_FOOD);
    add_lwvs(0, 400, 400, 7);
    add_reload("reload_steak", "init_steak");
}

void
init_steak(mixed *data)
{
    set_amount(data[0]);
    add_lwvs(0, data[1] - query_weight(),
	     data[2] - query_volume(),
	     data[3] - query_size());
}

mixed *
reload_steak()
{
    return ({ query_amount(), query_weight(), query_volume(), query_size() });
}
