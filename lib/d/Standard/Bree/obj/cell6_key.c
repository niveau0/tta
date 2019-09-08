inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("iron");
    set_pname("keys");
    set_short("iron key");
    set_pshort("iron keys");
    set_long("A heavy iron key. A small '6' in engraved on it.\n");
    set_lwvs(0, 50, 30, 3);
    add_prop(OBJ_MATERIAL, M_IRON);
    set_value(6);
}
