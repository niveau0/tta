/*
 * Function name: jump
 * Description:   jump around in the room
 * Arguments:     arg - the offsets to jump
 * Returns:       1 if success, -1 else
 */
static nomask int
jump(string arg)
{
    int x, y, z, *co;
    
    if (!arg || sscanf(arg, "%d %d %d", x, y, z) != 3)
    {
	tp->catch_tell("Usage: jump <xoffset> <yoffset> <zoffset>\n");
	return -1;
    }
    co = tp->query_coordinates()[..];
    co[0] += x;
    co[1] += y;
    co[2] += z;
    if (query_leaving_room(co) != this_object())
    {
	tp->catch_tell("You would leave the room with these offsets!\n");
	return -1;
    }
    tell_objects(LISTEN(this_object()) - ({ tp }),
		 ({ QCTNAME(tp), " jumps through the room.\n" }), MSG_SEE, tp);
    tp->catch_tell("Ok, you jumped.\n");
    tp->set_coordinates(co);  
    return 1;
}
