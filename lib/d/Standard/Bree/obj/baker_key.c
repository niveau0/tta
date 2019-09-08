inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("black");
    add_adj("small");
    set_pname("keys");
    set_short("small black key");
    set_pshort("small black keys");
    set_long("A small black key. The end is formed like a pretzel.\n");
    set_lwvs(0, 30, 18, 2);
    add_prop(OBJ_MATERIAL, M_IRON);
    set_value(6);
}
