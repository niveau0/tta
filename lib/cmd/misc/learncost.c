/*
 * Function name: learncost
 * Description:   list skill learncosts
 * Arguments:     str - the players argument
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
learncost(string str, object tp)
{
    int    sz;
    string *args;
    object ob;
    
    if (str && (sz = sizeof(args = explode(str," "))))
    {
	if (sz < 2 || _ACCESS_LEVEL(geteuid(tp)) < 1)
	    ob = tp;
	else if (!(ob = present(str = args[1], E(tp))) &&
		 !(ob = find_player(str)) && !(ob = find_living(str)) ||
		 !living(ob))
	{
	    tp->catch_tell("No such living.\n");
	    return -1;
	}
	smore(LEARN_OB->learncost_cmd(args[0], ob), tp);
	return 1;
    }
    smore(LEARN_OB->learncost_cmd("", tp), tp); /* show error msg */
    return 1;
}
