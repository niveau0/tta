inherit "/base/object";

#include <macros.h>
#include <properties.h>
#include <material.h>

static nomask void
create_object()
{
    set_name("bed");
    set_pname("beds");
    set_short("bed");
    set_pshort("beds");
    set_long_bs("A wooden bedstead, equipped with pillow, sheets and "+
	     "a blanket.\n");
    set_lwvs(0, 75000, 50000, 25);
    set_value(30);
    add_prop(OBJ_MATERIAL, M_OAK);
}
