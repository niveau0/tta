/* **************************************************************************
 * met - Set the metflag, a coder can decide if met or unmet with everyone
 */
nomask int
metflag(string str)
{
    object pl;

    CHECK_LEVEL(_BUILDER);

    if (!str)
	pl = this_interactive();
    else
	pl = find_player(str);
    
    if (pl)
    {
	pl->set_coder_unmet(0);
	this_interactive()->catch_tell("Ok.\n");
	return 1;
    }
    else
    {
	this_interactive()->catch_tell("Set met-flag for whom?\n");
	return -1;
    }
}
