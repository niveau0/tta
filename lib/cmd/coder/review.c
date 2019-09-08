/* **************************************************************************
 * review - review move messages
 */
nomask int
review(string str)
{
    object tp;

    CHECK_LEVEL(_BUILDER);
    
    if (!str || !strlen(str))
	tp = this_interactive();
    else
	tp = find_player(str);

    if (!tp) 
    {
	this_interactive()->catch_tell("No such player: " + str +".\n");
	return -1;
    }

    this_interactive()->
	catch_tell("teleport out: " + tp->query_teleport_out() +
		   "\nteleport in : " + tp->query_teleport_in() +
		   "\nwalk msg    : " + tp->query_move_msg() +
		   "\nrun msg     : " + tp->query_run_msg() + "\n");
    return 1;
}
