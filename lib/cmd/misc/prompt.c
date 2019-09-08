/*
 * Function name: prompt
 * Description:   set the prompt
 * Arguments:     str - what should be included in prompt
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
prompt(string str, object tp)
{
    int    f;
    string *args;
    object user;

    if (!str)
    {
	tp->set_prompt(0);
	tp->catch_tell("Cleared the prompt. Prompt may include:\n"+
		       " hp mana" +
		       (_ACCESS_LEVEL(geteuid(tp)) > 0 ? " path.\n" : ".\n"));
	return 1;
    }
    else
    {
	args = explode(str, " ");
	if (IN_ARRAY("path", args) && _ACCESS_LEVEL(geteuid(tp)) > 0)
	    f |= 8;
	if (IN_ARRAY("hp", args))
	    f |= 4;
	if (IN_ARRAY("mana", args))
	    f |= 2;
	tp->set_prompt(f);
    }
    tp->echo("You have set your new prompt.\n");
    return 1;
}
