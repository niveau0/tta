inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("bronze");
    add_adj("heavy");
    set_pname("keys");
    set_short("heavy bronze key");
    set_pshort("heavy bronze keys");
    set_long("A heavy bronze key.\n");
    set_lwvs(0, 40, 22, 6);
    add_prop(OBJ_MATERIAL, M_BRONZE);
    set_value(5);
}
