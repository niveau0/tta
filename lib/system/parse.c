#include <config.h>
#include <language.h>

private int last_parse_result;

/*
 * Function name: get_parse_result
 * Description:   get the amount of items that matched for last parse call
 * Returns:       integer
 */
nomask int
get_parse_result()
{
    return last_parse_result;
}

/*
 * Function name: parse
 * Description:   try to match a pattern with an objects names/identifiers
 * Arguments:     p - pattern to match
 *                list - object list
 *                tp - living (optional for introduce check)
 *                distance - optional for distance check
 * Returns:       array of matching objects
 */
static nomask varargs object *
parse(string p, object *list, object tp, int distance)
{
    int    i, f, t, type, lsz, num, *co;
    string s1, s2, *cmd;
    object *found;

    if (!p)
	error("Bad argument 1 for function parse");
    if (typeof(list) != T_ARRAY)
	error("Bad argument 2 for function parse");
    
    if (!(lsz = sizeof(list)))
	return ({ });
    if ((f=sizeof(cmd = explode(p, " and "))) > 1 ||
	(f=sizeof(cmd = explode(p, ", "))) > 1)
    {
	found = ({ });
	for (i=0; i<f; i++)
	    found |= parse(cmd[i], list, tp, distance);
	return found;
    }

    if (p == "me" && tp && member_array(tp, list) > -1)
	return ({ tp });
    
    if (p == "all")
    {
	if (!tp || distance <= MELEE_DISTANCE)
	    return list;
	co = tp->query_coordinates();
	while (lsz--)
	{
	    if (!distance(co, list[lsz]->query_coordinates(co), distance))
		list[lsz] = 0;
	}
	return list - ({ 0 });
    }

    if (strlen(p) > 4 && p[..3] == "the ")
	p = p[4..];
    
    /* extract possible numberword */
    if (sscanf(p, "%s %s", s1, s2) == 2)
    {
	if ((num = LANG_WNUM(s1)) || s1 == "all")
	{
	    p = s2;
	    if (!num) /* all */
		num = 99999999;
	}
    }
    if (!num)         /* match singular or plural */
	type = 3;
    else if (num > 1) /* match only plural */
	type = 2;
    else              /* match only singular */
	type = 1;
    cmd = explode(p, " ");
    if (tp && distance)
	co = tp->query_coordinates();
    for (i=f=0, found=allocate(lsz); i<lsz; i++)
    {
	if (!(t = list[i]->parse_object(cmd, type, co, distance, tp)))
	    continue;
	found[i] = list[i];
	if (!num)
	{
	    if (t & 2)
	    {
		type = 2; /* found plural name, parse plural now */
		num = 99999999;
	    }
	    else
		type = num = 1; /* found singular name, parse sing. now */
	}
	f++;
	if (f == num || f == -num)
	    break;
    }
    found -= ({ 0 });
    last_parse_result = f; /* speed up for parse_items() in cmdsoul */
    if (!f)
	return found;
    if (num > 0)
    {
	if (num == 99999999)
	    return found;
	for (i=0; i<f; i++)
	{
	    if (t = found[i]->query_heap_size())
	    {
		found[i]->split_heap(num);
		num -= t;
	    }
	    else
		num--;
	    if (num < 1)
		break;
	}
	if (i == f) /* might occur if num > f */
	    return found;
	return found[..i];
    }
    num = -num;
    for (i=lsz=0; i<f; i++)
    {
	if (t = found[i]->query_heap_size())
	{
	    lsz += t;
	    if (num <= lsz)
	    {
		found[i]->split_heap(1);
		return ({ found[i] });
	    }
	}
	else
	{
	    if (num <= ++lsz)
		return ({ found[i] });
	}
    }
    return ({ });
}
