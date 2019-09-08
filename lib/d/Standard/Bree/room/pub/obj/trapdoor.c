inherit "/lib/misc/special/gate";

#include <door.h>
#include <material.h>
#include <properties.h>

static void
create_gate()
{
    set_name("trapdoor");
    add_name("hole");
    add_adj("wooden");
    set_short("trapdoor");
    set_pshort("trapdoors");
    set_long_bs("Several boards have been nailed together and framed by "+
		"a piece of metal, leading to a really strong construction. "+
		"Surely you can put heavy objects on it without fearing it "+
		"to break.\n");
    set_status(DOOR_CLOSED);
    add_lwvs(0, 50000, 110000, 40);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_exits("cellar",
	      "back",1);
}
