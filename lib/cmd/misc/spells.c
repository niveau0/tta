/*
 * Function name: spell
 * Description:   get help about a spell
 * Arguments:     realm - realm of the spell
 *                list - spell list
 *                name - the spell name
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
spell(string realm, string list, string name, object tp)
{
    int     *data;
    string  spell, str, range;
    mapping known;

    catch(spell = (SPELL_DIR + "/" + realm +
		   "/spellpath")->query_path(list) + name);
    if (file_size(spell + ".c") < 1 || 
	!mappingp(known = tp->query_prop(LIVE_KNOWN_SPELLS)) ||
	known[realm][list] < spell->query_level())
    {
	tp->catch_tell("No help available for that spell.\n");
	return -1;
    }
    data = spell->query_spell_data();
    switch (data[SP_RANGE])
    {
      case SP_SELF:
	  range = "only yourself"; break;
      case SP_TOUCH:
	  range = "touch"; break;
      default:
	  range = (string)data[SP_RANGE] + " inch"; break;
    }

    str = "*----------------------------------------------------------" +
	  "------------------*\n\n  Description: " +
	  BS(spell->query_desc(), 75, 15)[15..] +
	  "\n  Range:       " + range +
	  "\n  Language:    " +
	  capitalize(SK_DESC[data[SP_MAGIC_LANGUAGE]]) +
	  "\n\n*--------------------------------------------------------"+
	  "--------------------*\n";
    smore(str, tp);
    return 1;
}

/*
 * Function name: spells
 * Description:   show known spells
 * Arguments:     str - the spellist
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
spells(string str, object tp)
{
    int     i, sz, lev, l;
    string  realm, dir, cur, table, list, spell, *spells;
    mapping known;

    table = "";
    if (!str)
    {
	tp->catch_tell("Usage: spells <realm> [<list>] [<spellname>]\n");
	return -1;
    }
    if (sscanf(str, "%s %s %s", realm, list, spell) == 3)
	return spell(realm, list, spell, tp);
    if (sscanf(str, "%s %s", realm, list) != 2)
    {
	realm = str;
	if (!mappingp(known = tp->query_prop(LIVE_KNOWN_SPELLS)) ||
	    !map_sizeof(known = known[realm]))
	    str = "You don't know any spellists of that realm.\n";
	else
	{
	    str = ("\n     =============================================="+
		   "=======================\n ---<             You know " +
		   "spells on the following spellists              >---\n"+
		   "     == Spellist ============ Level "+
		   "======= Spellist ============ Level ==\n\n");
	    spells = map_indices(known);
	    for (i=0, sz=sizeof(spells); i<sz; i++)
		table += sprintf("        %./-22s..%./3d\n",
				 spells[i], known[spells[i]]);
	    str += sprintf("%*78s", table) + "\n";
	}
	if (!map_sizeof(known = tp->query_prop(LIVE_TMP_SPELLKNOWLEDGE)))
	    str += "You have no additional spells in mind.\n";
	else		
	{
	    str += "You have the following spells in mind:\n";
	    spells = map_indices(known);
	    for (i=0, sz=sizeof(spells); i<sz; i++)
		table += sprintf("        %./-22s..%./3d\n",
				 spells[i], known[spells[i]]);
	    str += sprintf("%*78s", table);
	}
	tp->catch_tell(str);
	return 1;
    }
    if (!IN_ARRAY(realm, REALMS))
    {
	tp->catch_tell("'" + realm + "' is no realm.\n");
	return -1;
    }
    dir = SPELL_DIR + "/" + realm + "/spellpath";
    catch(dir = dir->query_path(list));
    spells = get_dir(dir + "*.c");
    if (!sizeof(spells) ||
	!mappingp(known = tp->query_prop(LIVE_KNOWN_SPELLS)) ||
	!(lev = known[realm][list]))
    {
	tp->catch_tell("You don't know any spells of such a spellist.\n");
	return -1;
    }
    str = sprintf("\n     ==================================================="+
		  "==================\n ---<  Realm: %-15s                   "+
		  "   List: %-15s  >---\n"+
		  "     == Spell =============== Level "+
		  "======= Spell =============== Level ==\n\n",
		  realm, list);
    for (i=0, sz=sizeof(spells); i<sz; i++)
    {
	l = strlen(spells[i]);
	cur = spells[i][..l-3];
	if ((l = (dir + cur)->query_level()) > lev)
	    continue;
	table += sprintf("        %./-22s..%./3d\n", cur, l);
    }
    str += sprintf("%*78s", table);
	
    tp->catch_tell(str);
    return 1;
}
