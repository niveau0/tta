/* **************************************************************************
 * objectlist - gives a list of loaded objects
 */
nomask int
objectlist(string str)
{
    CHECK_LEVEL(_CREATOR);

    smore(change_ticks(_SYSTEMD, 30000000, "object_list", str),
	  this_interactive());
    return 1;
}
