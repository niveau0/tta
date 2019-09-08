inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <stdproperties.h>

static nomask void
create_object()
{
    set_name("column");
    set_pname("columns");
    add_adj("light");
    set_short("column");
    set_pshort("columns");
    set_long("Lights, sparks and flashes "+
	     "wander beneath the column's surface and illuminate the "+
	     "surroundings with a bright but flickering light. It is "+
	     "impossible to touch it due to an invisible force that makes "+
	     "your limbs glide off.\n");
    set_lwvs(35000, 0, (int)(120.0*2.5*2131.0), 120);
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_MATERIAL, M_AIR);
}
