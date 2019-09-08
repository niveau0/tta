inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("steel");
    add_adj("heavy");
    set_pname("keys");
    set_short("heavy steel key");
    set_pshort("heavy steel keys");
    set_long("A heavy steel key. It ends in a form of a horseshoe.\n");
    set_lwvs(0, 40, 35, 6);
    add_prop(OBJ_MATERIAL, M_STEEL);
    set_value(10);
}
