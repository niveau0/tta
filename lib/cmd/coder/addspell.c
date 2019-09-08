/* **************************************************************************
 * addspell - grant a spellist to a living
 */
nomask int
addspell(string str)
{
    int     level;
    string  to, realm, splist;
    object  tp, ob, *list;
    mapping spells;
    
    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (!str || sscanf(str, "%s %s %d %s", realm, splist, level, to) != 4)
    {
	tp->catch_tell("Usage: addspell <realm> <list> <level> <target>\n");
	return -1;
    }
    if (!to)
	ob = tp;
    else if (sizeof(list = parse(to, I(E(tp), 2), 0)))
	ob = list[0];
    else if (!(ob = find_living(to)))
    {
	tp->catch_tell("No such living.\n");
	return -1;
    }
    if (catch(str = (SPELL_DIR + "/" + realm +
		     "/spellpath")->query_path(splist)) || !str ||
	file_size(str) != -2)
    {
	tp->catch_tell("No such spellist.\n");
	return -1;
    }
    if (!mappingp(spells = ob->query_prop(LIVE_KNOWN_SPELLS)))
	spells = ([ ]);
    if (!mappingp(spells[realm]))
	spells[realm] = ([ splist : level ]);
    else
	spells[realm][splist] = level;
    ob->add_prop(LIVE_KNOWN_SPELLS, spells);
    ob->catch_tell("Your spells have been changed.\n");
    tp->catch_tell("Ok.\n");
    return 1;
}
