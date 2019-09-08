static nomask void
do_type(string str)
{
    int i;
    
    if ((i = member_array(str, ROOM_TYPES)) == -1)
	tp->catch_tell("No such type.\n");
    else
    {
	type ^= ROOM_TYPES[i+1];
	tp->catch_tell("New type set (toggled).\n");
    }
}
