inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_adj("old");
    add_adj("rusty");
    add_adj("iron");
    set_pname("keys");
    set_short("iron key");
    set_pshort("iron keys");
    set_long("This key is old and its metal has rusty parts as wells as "+
	     "polished spots.\n");
    set_lwvs(0, 60, 18, 3);
    add_prop(OBJ_MATERIAL, M_IRON);
    set_value(6);
}
