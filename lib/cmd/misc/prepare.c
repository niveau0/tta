/*
 * Function name: prepare
 * Description:   prepare for an action
 * Arguments:     str - what to prepare
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
prepare(string str, object tp)
{
    int    rnds;
    string type, what;

    if (!str || sscanf(str, "%s %s", type, what) != 2)
    {
	tp->catch_tell("Prepare for what?\n"+
		       "E.g. prepare spells <rounds>\n");
	return -1;
    }
    if (type == "spells" && sscanf(what, "%d", rnds))
    {
	if (rnds > 2)
	{
	    rnds = 2;
	    tp->catch_tell("More than 2 rounds is too long.\n");
	}
	else if (rnds < 0)
	    rnds = 0;
	tp->add_prop(LIVE_PREPARE_RNDS, rnds);
	tp->echo("You will now prepare your spells " + rnds +
		 (rnds == 1 ? " round.\n" : " rounds.\n"));
	return 1;
    }
    tp->catch_tell("Prepare for what?\nE.g. prepare spells <rounds>\n");
    return -1;
}
