inherit "/base/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("meat");
    set_name("chunk");
    set_name("chunk of meat");
    set_short("chunk of meat");
    set_pshort("chunk of meat");
    set_long("This is a fresh chunk of meat.\n");
    set_plong("These are some fresh chunks of meat.\n");
    set_heap_id(MASTER);
    set_value(45);
    set_amount(200);
    add_prop(OBJ_MATERIAL, M_FOOD);
    add_lwvs(0, 400, 400, 7);
    add_reload("reload_meat", "init_meat");
}

void
init_meat(mixed *data)
{
    set_amount(data[0]);
    add_lwvs(0, data[1] - query_weight(),
	     data[2] - query_volume(),
	     data[3] - query_size());
}

mixed *
reload_meat()
{
    return ({ query_amount(), query_weight(), query_volume(), query_size() });
}
