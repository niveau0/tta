inherit "/lib/misc/special/gate";

#include <door.h>
#include <material.h>
#include <properties.h>

#define DOMAINPATH "/d/Rhudaur/Bree/"

void
create_gate()
{
    set_name("citygate");
    add_name("gate");
    add_adj("wooden");
    set_short("wooden citygate");
    set_pshort("wooden citygates");
    set_long_bs("This gate ist properly constructed from thick wooden beams. "+
		"\n");
    add_lwvs(0, 3000, 2000000, 160);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_key(DOMAINPATH+"obj/townkey_bree");
    set_status(DOOR_CLOSED);
    set_exits("westgate",
	      "eastroad_west4",1);
}
