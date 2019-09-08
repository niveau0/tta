//inherit "/base/object";
inherit "surface";

#include <macros.h>
#include <properties.h>
#include <material.h>

static nomask void
create_object()
{
    create_surface();
    set_surface(1000);
    
    set_name("table");
    set_pname("tables");
    set_short("table");
    set_pshort("tables");
    set_long("A wooden table with some space to put things on.\n");
    set_lwvs(0, 3000, 21600, 24);
    set_value(3);
    add_prop(OBJ_MATERIAL, M_OTHER);
}
