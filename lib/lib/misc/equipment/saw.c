inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

static nomask void
create_object()
{
    set_name("saw");
    set_pname("saws");
    set_short("saw");
    set_pshort("saws");
    set_long("A sharp, small iron saw.\n");
    set_lwvs(0, 1250, 500, 25);
    set_value(230);
    add_prop(OBJ_MATERIAL, M_IRON);
}
