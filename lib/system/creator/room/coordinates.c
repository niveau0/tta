static nomask void
do_coord(string str)
{
    int x, y, z;
    
    if (!str || sscanf(str, "%d,%d,%d", x, y, z) != 3)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    room_coords = ({ x, y, z });
    tp->set_coordinates(room_coords);
    tp->catch_tell("New coordinates set. You have been moved.\n");
}
