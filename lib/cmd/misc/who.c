/*
 * Function name: who
 * Description:   show players in game, only possible in OOC rooms.
 * Arguments:     str - whom to see in list
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
who(string str, object tp)
{
    int    i;
    string roomfile, out, metname;
    object *u;
    
    roomfile = E(tp)->query_room_file();
    if (_ACCESS_LEVEL(geteuid(tp)) < 1 &&
	(!roomfile || strlen(roomfile) <= strlen(OOCAREA) ||
	 roomfile[..strlen(OOCAREA)-1] != OOCAREA))
    {
	tp->catch_tell("Only possible in out-of-character areas.\n");
	return -1;
    }
    out = "";
    u = users();
    if (sizeof(u) == 1)
    {
	tp->catch_tell("You are the only one in the game.\n");
	return 1;
    }
    for (i=sizeof(u); i--;)
    {
	if (tp->query_met(u[i]->query_name()))
	    out += u[i]->query_presentation(tp) + "\n";
    }
    smore("There are " + sizeof(u) + " people in the game. Of these you " +
	  "know " + (out == ""
		     ? "noone.\n"
		     : "the following:\n" + sprintf("% --- *78s", out)), tp);
    return 1;
}
