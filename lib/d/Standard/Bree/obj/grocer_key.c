inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("brass");
    add_adj("small");
    set_pname("keys");
    set_short("small iron key");
    set_pshort("small iron keys");
    set_long("A small iron key.\n");
    set_lwvs(0, 22, 18, 2);
    add_prop(OBJ_MATERIAL, M_COPPER);
    set_value(8);
}
