inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

static nomask void
create_object()
{
    set_name("saddle");
    set_pname("saddles");
    add_adj("leather");
    set_short("leather saddle");
    set_pshort("leather saddles");
    set_long("A fine saddle, made of leather.\n");
    set_lwvs(0, 6000, 8000, 12);
    set_value(500);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
