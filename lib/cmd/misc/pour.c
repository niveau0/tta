/*
 * Function name: pour
 * Description:   pour out a liquid 
 * Arguments:     str - what should be poured out
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
pour(string str, object tp)
{
    int    res;
    object *container;
    
    if (!str || !sscanf(str, "out %s", str))
    {
	tp->catch_tell("Usage: pour out <container>\n");
	return -1;
    }
    container = I(tp, 1) + tp->visible(I(E(tp), 1));
    if (!sizeof(container = parse(str, container, tp, 0)))
    {
	tp->catch_tell("You can't see any '" + str + "'.\n");
	return -1;
    }
    if (!(res = container[0]->pour_out_cmd(tp)))
    {
	tp->catch_tell("You cannot pour out that item.\n");
	return -1;
    }
    return res;
}
