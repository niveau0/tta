/*
 * Function name: parse_cast
 * Description:   parse the cast command line
 * Arguments:     str - spell arguments (name, spellist, level, etc.)
 *                tp - acting living
 * Returns:       array with spell arguments, -1 if not successful
 */
nomask mixed
parse_cast(string str, object tp)
{
    int     lev, knownlev, splev;
    string  level, spell, realm, list, target, extra;
    mapping known;
    
    if (!str || sscanf(str, "'%s %s %s %s' %s", level, realm, list, spell,
		       target) != 5)
    {
	tp->catch_tell("Usage: cast '<power> <realm> <list> <spell>' " +
		       "<target|direction>\n", 1);
	return -1;
    }
    sscanf(spell, "%s %s", spell, extra);
    if (!IN_ARRAY(realm, REALMS))
	return -1;
    
    catch(spell =
	  (SPELL_DIR + "/" + realm + "/spellpath")->query_path(list) + spell);
    if (file_size(spell + ".c") < 1)
	return -1;

    /* check temporary spell knowledge first */
    if (mappingp(known = tp->query_prop(LIVE_TMP_SPELLKNOWLEDGE)) &&
	(knownlev = known[spell]) > 0)
    {
	/* power syllable not checked here */
	lev = knownlev;
	known[spell] = 0;
	tp->add_prop(LIVE_TMP_SPELLKNOWLEDGE, known);
    }
    else if (mappingp(known = tp->query_prop(LIVE_KNOWN_SPELLS)) &&
	     (knownlev = known[realm][list]) >= (splev = spell->query_level()))
    {
	if ((lev = member_array(level, OVERCAST_SYLLABLES)) != -1)
	    /* max overcast: knownlev + splev */
	    lev = knownlev + (knownlev - splev) * (++lev) /
		  sizeof(OVERCAST_SYLLABLES);
	else if ((lev = member_array(level, POWER_SYLLABLES)) != -1)
	    /* max standard power: knownlev */
	    lev = splev + (knownlev - splev) * lev / sizeof(POWER_SYLLABLES);
	else /* unknown power syllable */ 
	    return -1;
    }
    else
	return -1;
    
    return ({ spell, lev, knownlev, target, tp, extra });
}

/*
 * Function name: cast
 * Description:   cast a spell
 * Arguments:     str - spell arguments (name, spellist, level, etc.)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
cast(string str, object tp)
{
    mixed  args;

    if ((args = parse_cast(str, tp)) == -1)
    {
	tp->catch_tell("You stutter some useless words.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp),
			" utters something totally incomprehensible.\n" }),
		     MSG_HEAR, tp, 590);
	return -1;
    }
    if (tp->query_fatigue() < 10)
    {
	tp->catch_tell("You feel much too tired to concentrate on magic.\n");
	return -1;
    }
    tp->add_fatigue(-5);
    sscanf(str, "'%s'%*s", str);
    args[0]->do_spell(args[1], args[2], args[3], args[4], args[5],
		      "'" + str + "'");
    return 1;
}
