/* **************************************************************************
 * props - show all set properties of an object
 */
nomask int
props(string str)
{
    int    i;
    string out1, out2;
    object ob, tp, *obj;
    mixed  ind, curprops, value;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
	ob = tp;
    else if (str == "here")
	ob = E(tp);
    else if (sizeof(obj = parse(str, I(tp, 7) +
				I(E(tp), 0), 0)))
	ob = obj[0];
    else if (!(ob = find_living(str)) &&
	     !(ob = _SYSTEMD->finger_player(str)))
    {
	tp->catch_tell("No such object.\n");
	return -1;
    }

    if (!mappingp(curprops = ob->query_props()))
    {
	tp->catch_tell("No properties found.\n");
	return 1;
    }
    ind = map_indices(curprops);
    out1 = out2 = "";
    for (i=sizeof(ind); i--;)
    {
	value = curprops[ind[i]];
	if (pointerp(value) || mappingp(value) || objectp(value))
	    value = type_to_string(value, 20);
	value += "\n";
	
	if (props[ind[i]]) /* is within <properties.h> */
	    out1 += sprintf("%./-20s. %s", props[ind[i]], value);
	else
	{
	    if (intp(ind[i]))
		out2 += sprintf("%./-20d. %s", ind[i], value);
	    else
		out2 += sprintf("%./-20s. %s", ind[i], value);
	}
    }
    smore(out1 + out2, tp);
    return 1;
}
