inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("key");
    add_name("townkey");
    add_adj("metal");
    add_adj("big");
    set_pname("keys");
    set_short("finely crafted key");
    set_pshort("finely crafted keys");
    set_long_bs("The key is too large to fit into a usual lock of a door or "+
		"chest. The metal is smooth and polished. Fine lines and "+
		"decorations cover its surface.\n");
    set_lwvs(0, 800, 450, 11);
    add_prop(OBJ_MATERIAL, M_IRON);
    set_value(15);
}
