/*
 * Function name: appraise
 * Description:   appraise an item
 * Arguments:     str - what to appraise
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
appraise(string str, object tp)
{
    int    i, t, w, v, s, sz, sk, per, hour, *tarr;
    string short;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: appraise <item>\n");
	return -1;
    }
    sk = tp->query_skill_bonus(SK_APPRAISAL);
    if (sk > 100) /* use a fixed value for appraisal, not random */
	sk = 100;
    if (str == "time")
    {
	tp->skill_check(SK_APPRAISAL, 0, 0); /* care for skill usage */
	t = random(100 - sk) * TIME_SCALE - (100 - sk) * TIME_SCALE / 2;
	tarr = TIMEARRAY(t * 60);
	hour = tarr[HOUR_POS];
	if (NIGHT)
	{
	    /* if night, determine if near to dusk or dawn */
	    if (hour >= DAWN && hour <= DUSK)
	    {
		if (ABS(hour - DAWN) > ABS(hour - DUSK))
		    tarr[HOUR_POS] = DUSK+1;
		else
		    tarr[HOUR_POS] = DAWN-1;
	    }
	}
	else
	{
	    /* if day, determine if near to dusk or dawn */
	    if (hour < DAWN || hour > DUSK)
	    {
		if (ABS(hour - DAWN) > ABS(hour - DUSK))
		    tarr[HOUR_POS] = DUSK;
		else
		    tarr[HOUR_POS] = DAWN;
	    }
	}
	tp->catch_tell(TIMESTRING(tarr));
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " frowns.\n" }), MSG_SEE, tp);
	return 1;
    }
    list = I(tp, 7) + tp->visible(I(E(tp), 0));
    list = parse(str, list, tp);
    if (!(sz = sizeof(list)))
    {
	tp->catch_tell("No '" + str + "' to appraise here.\n");
	return -1;
    }
    tp->skill_check(SK_APPRAISAL, 0, 0); /* care for skill usage */
    str = "";
    sk = 100 - sk;
    per = 100 - sk / 2;
    
    for (i=0; i<sz; i++)
    {
	if (living(list[i]))
	    short = (list[i] == tp ? "you" : list[i]->query_nonmet_name());
	else
	    short = list[i]->short();
	w = (list[i]->query_weight() * (per + random(sk)) / 100);
	if (w > 100000)
	    w = w / 30000 * 30000;
	else if (w > 1000)
	    w = w / 1000 * 1000;
	v = (list[i]->query_volume() * (per + random(sk)) / 100);
	if (v > 100000)
	    v = v / 30000 * 30000;
	else if (v > 1000)
	    v = v / 1000 * 1000;
	s = (list[i]->query_size() * (per + random(sk)) / 100);
	str += BS("You assume that the " + short + " weight" +
		  (list[i] == tp ? " " : "s ") + w +
		  " gram with a volume of " + v + " milliliters. " +
		  (list[i] == tp ? "Your" :
		   capitalize(list[i]->query_possessive())) + " size " +
		  "is around " + s + " inch.\n\n", 75, 0);
    }
    tp->catch_tell(str);
    return 1;
}
