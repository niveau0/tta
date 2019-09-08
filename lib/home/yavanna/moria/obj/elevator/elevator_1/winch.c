inherit "../base/winch";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <event.h>
#include "../../../include/moria.h"

void
create_winch()
{   set_name("winch");
    add_adj("wooden");
    set_short("wooden winch");
    set_pshort("wooden winches");
    set_long("A winch is here, fixed to the wall.\n"+
	     "Maybe you can turn it.\n");
    add_prop(OBJ_MATERIAL,M_OAK);
    add_lwvs(0,100,100,50);
}
