/*
 * Function name: parry
 * Description:   set parry weapon
 * Arguments:     str - which should be the parry weapon
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
parry(string str, object tp)
{
    object *list;
    
    if (!str)
    {
	tp->catch_tell("Usage: parry <weapon>\n");
	return -1;
    }
    list = I(tp, 4);
    list = parse(str, list, 0);
    if (!check_size(list, "No '" + str + "' to parry with.\n", tp))
	return -1;
    if (list[0]->query_wielder() != tp)
    {
	tp->catch_tell("You don't wield such a weapon.\n");
	return -1;
    }
    if (list[0]->query_shield())
    {
	tp->catch_tell("Shields work as extra defense. Choose a weapon.\n");
	return -1;
    }
    tp->cb_set_parry_weapon(list[0]);
    tp->echo("Ok, you will now parry with your " + str + ".\n");
    return 1;
}
