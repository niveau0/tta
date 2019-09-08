/*
 * Function name: present
 * Description:   search an object in an inventory
 * Arguments:     item - item name or object pointer to search
 *                obj - object to search in (inventory)
 * Returns:       object if found, 0 else
 */
static nomask object
present(mixed item, object obj)
{
    int    i, sz;
    object *inv;

    if (!item)
	BADARG(1, present);
    if (!obj)
	BADARG(2, present);
    if (typeof(item) == T_OBJECT)
    {
	if (item->environment() == obj)
	    return item;
    }
    if (typeof(item) != T_STRING)
	BADARG(1, present);
    for (i=0, sz=sizeof(inv=obj->inventory(0)); i<sz; i++)
    {
    	if (inv[i]->id(item))
	    return inv[i];
    }
    return 0;
}

/*
 * Function name: process_value
 * Description:   process Value By Function Call stringformat
 * Arguments:     arg - VBFC string, format:
 *                      "@@function:path|optinonal argument|...@@"
 * Returns:       return value of function, else 'arg' string
 */
static nomask mixed
process_value(string arg)
{
    string file, func, *args;
    object ob;
    mixed  res;

    if (!arg)
	BADARG(1, process_value);
    if (strlen(arg) < 3 || arg[0] != '@' || arg[1] != '@')
	return arg;
    if (sscanf(arg[2..], "%s:%s", func, file) != 2)
	return call_other(_VBFC, "vbfc", this_object(), arg[2..]);
    args = explode(file, "|");
    file = args[0];
    if (!(ob = find_object(file)))
    {
    	catch(ob = compile_object(file));
    	if (!ob)
    	    return 0;
    }
    args = args[1..];
    return call_other(_VBFC, "vbfc", ob, func, args...);
}

/*
 * Function name: sort_array
 * Description:   sort an array by sort function
 * Arguments:     source - source array to sort
 *                func - function to call (to determine sort order)
 *                obj - object to call function in
 * Returns:       array, sorted
 */
static nomask mixed *
sort_array(mixed *source, string func, mixed obj)
{
    int   i, j, index1, index2, end1, end2, step, halfstep, size;
    mixed *dest, *temp;

    if (!source)
	BADARG(1, sort_array);
    if (!func)
	BADARG(2, sort_array);

    if (typeof(obj) == T_STRING)
	obj = compile_object(obj);
    else if (typeof(obj) != T_OBJECT)
	BADARG(3, sort_array);

    if ((size = sizeof(source)) < 2)
	return source[..];
    dest = allocate(size);
    step = 2;
    halfstep = 1;

    while (halfstep < size)
    {
	for (i = j = 0; i < size; i += step)
	{
	    index1 = i;
	    index2 = i + halfstep;
	    end1 = index2;
	    if (end1 > size)
		end1 = size;
	    end2 = i + step;
	    if (end2 > size)
		end2 = size;

	    while (index1 < end1 && index2 < end2)
	    {
		if (call_other(obj, func, source[index1], source[index2]) > 0)
		    dest[j++] = source[index2++];
		else
		    dest[j++] = source[index1++];
	    }
	    if (index1 == end1)
	    {
		while (index2 < end2)
		    dest[j++] = source[index2++];
	    }
	    else
	    {
		while (index1 < end1)
		    dest[j++] = source[index1++];
	    }
	}
	halfstep = step;
	step += step;
	temp = source;
	source = dest;
	dest = temp;
    }
    return source;
}

/*
 * Function name: roll_dice
 * Description:   return a random value (open end roll included)
 * Arguments:     f - flag, if negative roll is also negative open ended
 * Returns:       integer
 */
static varargs nomask int
roll_dice(int f)
{
    int d, t;

    t = d = random(100) + 1;

    if (t > 95)
    {
	t = random(100) + 1;
	d += t;
	while (t > 95)
	{
	    t = random(100) + 1;
	    d += t;
	}
	return d;
    }

    if (f == -1 && t < 6)
    {
	t = random(100) + 1;
	d -= t;
	while (t < 6)
	{
	    t = random(100) + 1;
	    d -= t;
	}
	return d;
    }
    return d;
}

/*
 * Function name: in_range
 * Description:   check if a target is in range of a given 3D cube
 * Arguments:     co_tar - target coordinates
 *                co_ob - cube base coordinates
 *                range - cube size values
 * Returns:       1 if in range, 0 else
 */
static nomask int
in_range(int *co_tar, int *co_ob, int *range)
{
    if (!co_ob || !range || !co_tar)
	return 0;

    if (co_tar[0] < co_ob[0] || co_tar[1] < co_ob[1] || co_tar[2] < co_ob[2] ||
	co_tar[0] >= co_ob[0] + range[0] || co_tar[1] >= co_ob[1] + range[1] ||
	co_tar[2] >= co_ob[2] + range[2])
	return 0;
    return 1;
}

/*
 * Function name: distance
 * Description:   check if distance between two objects is lesser or equal max
 * Arguments:     co_ob - first objects coordinates
 *                co_tar - second objects coordinates
 *                max - maximum distance value
 * Returns:       1 if distance is <= max, else 0
 */
static nomask int
distance(int *co_ob, int *co_tar, int max)
{
    int x;

    if (!co_ob || !co_tar)
	return 1;

    if ((x = co_ob[0] - co_tar[0]) < 0)
	x = -x;
    if (x > max)
	return 0;

    if ((x = co_ob[1] - co_tar[1]) < 0)
	x = -x;
    if (x > max)
	return 0;

    if ((x = co_ob[2] - co_tar[2]) < 0)
	x = -x;
    if (x > max)
	return 0;

    return 1;
}

/*
 * Function name: direct_approach
 * Description:   try to move a living up to melee distance
 * Arguments:     tp - moving living
 *                ob - living to move to
 * Returns:       1 if success, 0 else
 */
static nomask int
direct_approach(object tp, object ob)
{
    int f, *co_ob, *co_tp;

    if (!(co_tp = tp->query_coordinates()) ||
	!(co_ob = ob->query_coordinates(co_tp)))
	return 1;

    if (!distance(co_tp, co_ob, tp->query_step()))
	return 0;

    if (co_tp[0] - co_ob[0] > MELEE_DISTANCE)
    {
	co_tp[0] = co_ob[0] + MELEE_DISTANCE - 1;
	f = 1;
    }
    else if (co_ob[0] - co_tp[0] > MELEE_DISTANCE)
    {
	co_tp[0] = co_ob[0] - MELEE_DISTANCE + 1;
	f = 1;
    }
    if (co_tp[1] - co_ob[1] > MELEE_DISTANCE)
    {
	co_tp[1] = co_ob[1] + MELEE_DISTANCE - 1;
	f = 1;
    }
    else if (co_ob[1] - co_tp[1] > MELEE_DISTANCE)
    {
	co_tp[1] = co_ob[1] - MELEE_DISTANCE + 1;
	f = 1;
    }
    if (f)
	tp->catch_tell("You approach to near distance.\n");
    return 1;
}
