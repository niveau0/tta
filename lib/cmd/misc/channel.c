/*
 * Function name: channel
 * Description:   channel mana to someone
 * Arguments:     str - how much to channel
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
channel(string str, object tp)
{
    int    sz, percent, mana;
    string to, amount;
    object here, *list;

    if (!str)
    {
	tp->catch_tell("Usage: channel <few|some|much> to <name>\n");
	return -1;
    }
    if (sscanf(str, "%s to %s", amount, to) != 2)
    {
	tp->catch_tell("Channel how much mana to whom?\n");
	return -1;
    }
    switch (amount)
    {
      case "few":
	percent = 10; break;
      case "some":
	percent = 30; break;
      case "much":
	percent = 50; break;
      default:
	tp->catch_tell("'" + amount + "' is no valid amount. Try few, some " +
		       "or much.\n");
	return -1;
    }
    here = E(tp);
    list = tp->visible(I(here, 2));
    list = parse(to, list, tp);
    if (!check_size(list, "No '" + to + "' to channel to.\n", tp))
	return -1;
    tell_objects(LISTEN(here) - ({ tp }),
		 ({ QCTNAME(tp), " closes the eyes for a short time and " +
			"looks concentrated.\n" }), MSG_SEE, tp);
    mana = tp->query_mana() * percent / 100;
    if (tp->skill_check(SK_CHANNELING, -percent, -1) < 0)
    {
	tp->catch_tell("You failed to channel your mana.\n");
	tp->add_mana(-mana * 2);
    }
    else
    {
	tp->catch_tell("Good, you succeed.\n");
	if (mana > 0)
	{
	    list[0]->catch_tell("Someone channeled you some mental power.\n");
	    tp->add_mana(-mana);
	    list[0]->add_mana(mana);
	}
    }
    return 1;    
}
