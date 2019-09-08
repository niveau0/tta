/* **************************************************************************
 * shutdown - shut the game down
 */
nomask int
shutdown_game(string str)
{
    string *argv;
    object tp;

    CHECK_LEVEL(_CREATOR);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: shutdown <time> <reason>\n");
	return 1;
    }
    argv = explode(str, " ");
    if (sizeof(argv) == 1)
    {
	if (argv[0] == "abort")
	{
	    _SYSTEMD->stop_shutdown();
	    return 1;
	}
	tp->catch_tell("Invalid argument to shutdown: " + str + "\n");
	return 1;
    }
    _SYSTEMD->shutdown_game(this_interactive(), argv[0],
			      implode(argv[1..], " "));
    return 1;
}
