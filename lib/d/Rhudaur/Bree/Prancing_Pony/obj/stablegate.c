inherit "/lib/misc/special/gate";

#include <door.h>
#include <material.h>
#include <properties.h>

void
create_gate()
{
    set_name("stablegate");
    add_name("gate");
    add_adj("wooden");
    set_short("wooden stablegate");
    set_pshort("wooden stablegates");
    set_long_bs("This gate ist properly constructed from thick wooden beams "+
		"and light brown boards.\n");
    add_lwvs(0, 3000, 2000000, 155);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_status(DOOR_OPEN);
    set_exits("stable2", "yard" ,1);
}
