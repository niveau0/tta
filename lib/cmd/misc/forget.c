/*
 * Function name: forget
 * Description:   forget someones name
 * Arguments:     the one to remember
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
forget(string str, object tp)
{
    object ob;
    
    if (!str)
    {
	tp->catch_tell("Usage: forget <name>\n");
	return -1;
    }
    if (!tp->remove_remembered(str))
    {
	tp->catch_tell("You do not seem to remember anyone called " +
		       str + ".\n");
	return 1;
    }
    tp->echo("You forgot " + str + ".\n");
    return 1;
}
