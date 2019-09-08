inherit "../base/elevator_door";

#include <composite.h>
#include <door.h>
#include <filepath.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <skills.h>
#include <config.h>
#include <properties.h>
#include <event.h>
#include "../../../include/moria.h"


static void
create_elevator_door()
{
    set_name("door");
    add_adj("stone");
    set_short("stone door");
    set_pshort("stone doors");
//    set_direction_desc("");
    set_long("A two-winged stone door.\n");
    set_status(DOOR_CLOSED);
    add_prop(OBJ_MATERIAL,M_STONE);
    add_lwvs(0, 50000, 100000, 80);
}

