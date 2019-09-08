/* **************************************************************************
 * demote - demote a someone to a lower level
 */
nomask int
demote(string arg)
{
    int     lev;
    string  levname, name;
    object  ob, tp;

    CHECK_LEVEL(_ADMIN);

    tp = this_interactive();
    if (!arg || sscanf(arg, "%s %s", name, levname) != 2)
    {
	tp->catch_tell("Usage: demote <playername> <newlevel>\n");
	return -1;
    }
    if (levname == "mortal")
	lev = 0;
    else if (levname == "builder")
	lev = _BUILDER;
    else
    {
	tp->catch_tell("No such level, choose mortal or builder.\n");
	return -1;
    }
    if (_ACCESS_LEVEL(name) <= lev)
    {
	tp->catch_tell("Demote is used for decreasing levels.\n");
	return -1;
    }
    tp->catch_tell(_SYSTEMD->change_coder_level(name, lev) + "\n");
    if (ob = find_player(name))
    {
	_SYSTEMD->remove_player(ob);
	_SYSTEMD->add_player(ob);
    }
    return 1;
}
