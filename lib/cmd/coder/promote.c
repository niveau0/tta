/* **************************************************************************
 * promote - promote someone to a higher level.
 */
nomask int
promote(string arg)
{
    int    lev;
    string levname, name;
    object ob, tp;

    CHECK_LEVEL(_ADMIN);

    tp = this_interactive();
    if (!arg || sscanf(arg, "%s %s", name, levname) != 2)
    {
	tp->catch_tell("Usage: promote <playername> <newlevel>\n");
	return -1;
    }
    if (levname == "builder")
	lev = _BUILDER;
    else if (levname == "creator")
	lev = _CREATOR;
    else
    {
	tp->catch_tell("No such level, choose builder or creator.\n");
	return -1;
    }
    if (!_SYSTEMD->exist_player(name))
    {
	tp->catch_tell("Can't find player " + name + "\n");
	return -1;
    }
    if (_ACCESS_LEVEL(name) >= lev)
    {
	tp->catch_tell("Promote is used for increasing levels.\n");
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
