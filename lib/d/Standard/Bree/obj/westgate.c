inherit "/lib/misc/special/gate";

#include <door.h>
#include <material.h>
#include <properties.h>

#define DOMAINPATH "/d/Standard/Bree/"

void
create_gate()
{
    set_name("citygate");
    add_name("gate");
    add_adj("huge");
    add_adj("wooden");
    set_short("huge citygate");
    set_pshort("huge citygates");
    set_long_bs("This gate ist properly constructed from thick wooden beams "+
		"and supporting metalparts. It should resist most earthly "+
		"powers long enough to guarantee the person behind it a long "+
		"life.\n");
    add_lwvs(0, 10000, 6000000, 200);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_key(DOMAINPATH+"obj/town_key");
    set_status(DOOR_LOCKED);
    set_exits("westgate",
	      "westgate_outside",1);
}
