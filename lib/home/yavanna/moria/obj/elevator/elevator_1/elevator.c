inherit "../base/elevator";

#include <filepath.h>
#include <macros.h>
#include <properties.h>
#include <room.h>
#include <event.h>
#include "../../../include/moria.h"

#define ROOM_LONG \
"A room nearly square in shape. You can see a winch here.\n"
#define ROOM_SHORT \
"A elevator"

static void
create_elevator()
{
    set_move_speed(50);
    set_long(ROOM_LONG);
    set_short(ROOM_SHORT);
    room_coords =({750, -650, 200 });
    room_file = MORIA_OBJ+"elevator/elevator_1/elevator.o";
}





