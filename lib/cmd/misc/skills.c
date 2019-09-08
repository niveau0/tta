/*
 * Function name: skills
 * Description:   show skills of a living, coders could see skills of others
 * Arguments:     str - skillgroup and optional player name
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
skills(string str, object tp)
{
    string s1, s2;
    object ob, *obj;
    
    if (str)
    {
	if (sscanf(str, "%s %s", s1, s2) == 2 &&
	    _ACCESS_LEVEL(geteuid(tp)) > 0)
	{
	    if (sizeof(obj = parse(s2, I(E(tp), 2), 0)))
		ob = obj[0];
	    else if (!(ob = find_living(s2)) &&
		     !(ob = _SYSTEMD->finger_player(s2)))
	    {
		tp->catch_tell("No such living.\n");
		return -1;
	    }
	    smore(ob->skill_living(s1), tp);
	}
	else
	    smore(tp->skill_living(str), tp);
	return 1;
    }
    smore(tp->skill_living(), tp);
    return 1;
}
