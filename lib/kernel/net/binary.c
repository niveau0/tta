/*
 * NAME:	open()
 * DESCRIPTION:	start a user connection on this object
 */
static void
open()
{
    //    message("Currently the binary port is not available. Bye.\n");
    //    destruct_object();
}

/*
 * NAME:	close()
 * DESCRIPTION:	close the user connection on this object
 */
nomask void
close(int f)
{
    if (!f)
	destruct_object();
}

/*
 * NAME:	receive_message()
 * DESCRIPTION:	receive user input
 */
static nomask void
receive_message(string str)
{
    debug("manwe", str);
    set_this_player(0);
}
