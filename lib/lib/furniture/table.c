inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

static nomask void
create_object()
{
    set_name("table");
    set_pname("tables");
    set_short("table");
    set_pshort("tables");
    set_long("A wooden table.\n");
    set_lwvs(0, 3000, 21600, 24);
    set_value(30);
    add_prop(OBJ_MATERIAL, M_OAK);
}
