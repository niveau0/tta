/*********************************************************************
 * Create a new room or edit an existing one.
 */
inherit "/base/room";

#include "room/creator.h"
#include "room/base.c"

static nomask int save_this();
static nomask int docu(string arg);
static nomask int edit();
static nomask string view_result(int choice);

static nomask void
create_room()
{
    room_short = "In a new room";
    room_long  = "A new room. That's what you would call empty.\n";
    xsize      = 10;
    ysize      = 10;
    height     = DEFAULT_ROOM_HEIGHT;
    light      = 0;
    type       = 0;
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
    save();
    tp->catch_tell("Room saved.\n");
    return 1;
}
