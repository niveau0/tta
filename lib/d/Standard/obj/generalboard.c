inherit "/base/board";

#include <material.h>
#include <properties.h>

#define LONG \
"A dark reflecting marble framed board.  There are lots of slips of paper\n"+\
"to write on, and some pins to stick those into the board.  At the top of\n"+\
"the board is a label reading 'General Messages'.\n"

static nomask void
create_board()
{
    set_name("pinboard");
    set_short("pinboard");
    set_pshort("pinboards");
    set_long(LONG);
    set_group("ooc.general");
    add_prop(OBJ_MATERIAL, M_STONE);
}
