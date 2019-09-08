/*
 * Function name: buy
 * Description:   buy something
 * Arguments:     str - what to buy
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
buy(string str, object tp)
{
    if (!str)
    {
	tp->catch_tell("Usage: buy <item>\n");
	return -1;
    }
    if (!E(tp)->buy_cmd(str))
    {
	tp->catch_tell("There is nothing to buy here.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
