/* **************************************************************************
 * uptime - get local time & date + uptime information
 */
nomask int
uptime()
{
    mixed st;
    
    CHECK_LEVEL(_BUILDER);

    st = status();
    this_interactive()->
	catch_tell("Local time:            " + ctime(time()) +
		   "\nStart time:            " + ctime(st[ST_STARTTIME]) +
		   "\nBoot  time:            " + ctime(st[ST_BOOTTIME]) +
		   "\nMudtime:               " + TIMEDESC +
		   "Driver time:           " +
		   (CONV_REALTIME(time() - status(find_object(_DRIVER))
				  [O_COMPILETIME])) +
		   "\nUp time:               " +
		   CONV_REALTIME(st[ST_UPTIME]) + "\n");
    return 1;
}
