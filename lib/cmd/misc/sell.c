/*
 * Function name: sell
 * Description:   sell something
 * Arguments:     str - what to sell
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sell(string str, object tp)
{
    if (!str)
    {
	tp->catch_tell("Usage: sell <item>\n");
	return -1;
    }
    if (!E(tp)->sell_cmd(str))
    {
	tp->catch_tell("There is nothing where you can sell here.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
