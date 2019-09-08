inherit "/base/food";

#include <properties.h>
#include <macros.h>

static void
create_food()
{
    set_name("small cake");
    set_name("cake");
    set_short("small cake");
    set_pshort("small cakes");
    set_long("This is a fine small cake. Smells like your mother made it.\n");
    set_plong("These are some fine small cakes. Smells like your mother "+
	      "made them.\n");
    set_heap_id(MASTER);
    set_value(50);
    set_amount(400);
    add_lwvs(0, 310, 400, 3);
}
