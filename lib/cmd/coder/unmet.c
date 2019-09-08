/* **************************************************************************
 * unmet - Remove the metflag, a coder can decide to unmet with everyone
 */
nomask int
unmetflag(string str)
{
    object tp, pl;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
	pl = tp;
    else
	pl = find_player(str);
    
    if (pl)
    {
	pl->set_coder_unmet(1);
	tp->catch_tell("Ok.\n");
	return 1;
    }
    else
    {
	tp->catch_tell("Remove met-flag for whom?\n");
	return -1;
    }
}
