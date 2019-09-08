static nomask void
do_border(string str)
{
    int north, east, south, west;
    
    if (!str || sscanf(str, "%d,%d,%d,%d", north, east, south, west) != 4)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    edge_base[ROOM_EDGE_N] = north;
    edge_base[ROOM_EDGE_E] = east;
    edge_base[ROOM_EDGE_S] = south;
    edge_base[ROOM_EDGE_W] = west;
    tp->catch_tell("New borderlevels set.\n");
}
