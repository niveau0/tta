/*
 * Function name: detach
 * Description:   detach an item if possible
 * Arguments:     str - what to detach
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
detach(string str, object tp)
{
    if (!str || sscanf(str, "%*s from %*s") != 2)
    {
	tp->catch_tell("Usage: detach <item> from <item>\n");
	return -1;
    }
    return get(str, tp);
}
