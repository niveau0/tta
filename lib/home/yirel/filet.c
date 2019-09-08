inherit "/home/yirel/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("filet");
    set_short("filet");
    set_pshort("filets");
    set_long("This is the best cut of beef you can get from a corpse.\n");
    set_plong("These are the best cuts of beef you can get from a corpse.\n");
    set_heap_id(MASTER);
    set_value(325);
    set_amount(400);
    add_prop(OBJ_MATERIAL, M_FOOD);
    add_lwvs(0, 100, 120, 3);
}

void
init_reload(string s)
{
    int amount, weight, volume, size;
    
    sscanf(s, "%d#%d#%d#%d&&%s", amount, weight, volume, size, s);
    set_amount(amount);
    add_lwvs(0, weight - query_weight(), volume - query_volume(),
	     size - query_size());
    ::init_reload(s);
}

string
reload_data()
{
    return query_amount() + "#" + query_weight() + "#" + query_volume() +
	"#" + query_size() + "&&" + ::reload_data();
}
