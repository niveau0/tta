inherit "/base/stair";

#include <door.h>
#include <material.h>
#include <properties.h>

#define BREE_PATH "/d/Rhudaur/Bree"

void
create_stair()
{
    set_name("stair");
    add_adj("wooden");
    set_short("wooden stairway");
    set_pshort("wooden stairways");
    set_long("The stair is made from dark wooden boards. You see scratches"+
	     "and other signs of usage everywhere on it.\n");
    add_lwvs(0, 300000, 250000, 156);
    set_value(150);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_connection(BREE_PATH+"/Prancing_Pony/stairway32");
}

int *
query_range()
{
    return ({ 132, 36, 156 });
}
