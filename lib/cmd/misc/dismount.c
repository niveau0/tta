/*
 * Function name: dismount
 * Description:   try to dismount an object (a horse)
 * Arguments:     str - what to dismount
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
dismount(string str, object tp)
{
    object ob;

    if (!(ob = tp->query_prop(LIVE_MOUNTED)))
    {
	tp->catch_tell("You are not sitting on a horselike creature.\n");
	return -1;
    }
    return ob->dismount_cmd(tp);
}
