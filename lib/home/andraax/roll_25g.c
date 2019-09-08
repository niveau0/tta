inherit "/base/food";

#include <properties.h>
#include <macros.h>

static void
create_food()
{
    set_name("roll");
    add_adj("fresh");
    set_short("fresh roll");
    set_pshort("fresh rolls");
    set_long("A yummy fresh roll smells invitingly.\n");
    set_plong("Some yummy fresh rolls smell invitingly.\n");
    set_heap_id(MASTER);
    set_value(2);
    set_amount(25); /* gram */
    add_lwvs(0, 20, 100, 3);
}
