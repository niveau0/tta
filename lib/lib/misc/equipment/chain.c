inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

static nomask void
create_object()
{
    set_name("chain");
    set_pname("chains");
    add_adj("rusty");
    set_short("chain");
    set_pshort("chains");
    set_long("A ten feet long, rusty iron chain.\n");
    set_lwvs(0, 9000, 4500, 7);
    set_value(60);
    add_prop(OBJ_MATERIAL, M_IRON);
}
