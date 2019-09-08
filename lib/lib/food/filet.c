inherit "/base/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("filet");
    set_short("filet");
    set_pshort("filets");
    set_long("This is the best beef you can get from a corpse.\n");
    set_plong("These are the best beefs you can get from a corpse.\n");
    set_heap_id(MASTER);
    set_value(325);
    set_amount(400);
    add_prop(OBJ_MATERIAL, M_FOOD);
    add_lwvs(0, 100, 120, 3);
    add_reload("reload_filet", "init_filet");
}

void
init_filet(mixed *data)
{
    set_amount(data[0]);
    add_lwvs(0, data[1] - query_weight(),
	     data[2] - query_volume(),
	     data[3] - query_size());
}

mixed *
reload_filet()
{
    return ({ query_amount(), query_weight(), query_volume(), query_size() });
}
