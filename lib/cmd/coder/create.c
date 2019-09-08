/*
 * Function name: cr_create
 * Description:   clone a new `creator' object and call cr_create in it,
 *                thus creating a new room.
 * Arguments:     arg - additional command argument
 */
nomask int
cr_create(string arg)
{
    string room, *split;
    object tp, ob;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_player();
    if (arg)
    {
	split = explode(arg, " ");
	if (sizeof(split) < 2)
	    room = E(tp)->query_room_file();
	else
	    room = split[1];
	if (split[0] == "room")
	    ob = clone_object(CREATE_ROOM);
	else if (split[0] == "shop")
	    ob = clone_object(CREATE_SHOP);
	if (ob)
	{
	    seteuid(geteuid(), ob);
	    return ob->cr_create(room);
	}
    }
    tp->catch_tell("Usage: create shop|room [<path>]\n");
    return -1;
}
