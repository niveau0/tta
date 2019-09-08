/*
 * Function name: hp
 * Description:   show hitpoints, mana and fatigue
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
hp(string str, object tp)
{
    int    i, max;
    string out;

    if ((max = tp->query_max_hp()) < (i = tp->query_hp()) || max == 0)
	i = 8;
    else
    {
	i = i * 8 / max;
	if (i < 0) i = 0;
    }
    out = "You are physically " + health[i];

    if ((max = tp->query_max_mana()) < (i = tp->query_mana()) || max == 0)
	i = 8;
    else
    {
	i = i * 8 / max;
	if (i < 0) i = 0;
    }
    out += " and mentally " + mana[i] + ".\n";
    
    if ((max = tp->query_max_fatigue()) < (i = tp->query_fatigue()) ||
	max == 0)
	i = 10;
    else
    {
	i = i * 10 / max;
	if (i < 0) i = 0;
    }
    out += "You are " + fatigue[i] + ".\n";
    
    out += tp->query_blood_desc(tp) + tp->query_wound_desc(tp);
    tp->catch_tell(out);
    return 1;
}
