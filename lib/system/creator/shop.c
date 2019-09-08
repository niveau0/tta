/*********************************************************************
 * Create a new shop or edit an existing one.
 */
inherit "/base/town/shop";

#define SHOP

#include "room/creator.h"
#include "room/base.c"

static nomask void
create_shop()
{
    room_short = "In a new shop";
    room_long  = "A new shop. That's what you would call empty.\n";
    xsize      = 10;
    ysize      = 10;
    height     = DEFAULT_ROOM_HEIGHT;
    light      = 0;
    type       = TYPE_SHOP;
    climate    = 0;

    room_coords = ({ -9999999, -9999999, -9999999 });
}

/*
 * Function name: cr_create
 * Description:   Gets called by the command `create' (CODER_SOUL). Starts
 *                room editing
 * Arguments:     str - room path of room to create or edit
 * Returns:       1 if success, -1 else
 */
nomask int
cr_create(string str)
{
    if (!IS_CLONE)
	return -1;
    init_base(this_player(), str);
    return 1;
}

/*
 * Function name: save_this
 * Description:   save the edited room
 * Returns:       1 if successful, -1 else
 */
static nomask int
save_this()
{
    type |= TYPE_SHOP;
    save();
    tp->catch_tell("Shop saved.\n");
    return 1;
}
