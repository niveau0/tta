inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <stdproperties.h>

static nomask void
create_object()
{
    set_name("sphere");
    set_pname("spheres");
    add_adj("light");
    add_adj("half");
    set_short("half sphere");
    set_pshort("half spheres");
    set_long("The sphere is partly of a dark gray tone and partly filled "+
	     "with colourful sparks and lines. Your examining touch is "+
	     "prevented from reaching its surface, as it is dragged to "+
	     "the side by an invisible force.\n");
    set_lwvs(35000, 0, 35500, 10);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_NO_GET, 1);
}
