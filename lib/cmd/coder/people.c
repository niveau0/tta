/* **************************************************************************
 * people list
 */
nomask int
people(string arg)
{
    int    opt_u;
    string *opts;
    
    CHECK_LEVEL(_BUILDER);

    if (!arg)
	arg = "";
    
    opts = explode(arg, "");
    opt_u = IN_ARRAY("u", opts);
    
    if (IN_ARRAY("m", opts))
	smore(_SYSTEMD->userlist(opt_u), this_interactive());
    else
	this_interactive()->catch_tell(_SYSTEMD->userlist(opt_u));
    return 1;
}
