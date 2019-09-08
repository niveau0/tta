private mapping events;

/*
 * Function name: add_event
 * Description:   add this object to an event list
 * Arguments:     event - event type
 *                func - function to call in this object if this event occurs
 *                obj - object which should send events to this object
 */
static nomask void
add_event(int event, string func, object obj)
{
    if (!event || !func || !obj)
	return;
    rlimits (-1; -1)
    {
	obj->_F_add_event(event, func, this_object());
    }
}

/*
 * Function name: remove_event
 * Description:   stop listening to an event
 * Arguments:     event - event type
 *                func - function to remove from event calling list
 *                obj - object which should stop sending an event
 */
static nomask void
remove_event(int event, string func, object obj)
{
    if (!event || !obj)
	return;
    rlimits (-1; -1)
    {
	obj->_F_remove_event(event, func, this_object());
    }
}

/*
 * Function name: query_event
 * Description:   give all objects listening to an event
 * Arguments:     event - event type to query
 * Returns:       array, objects
 */
nomask object *
query_event(int event)
{
    if (!events || !events[event])
	return ({ });
    return map_indices(events[event]);
}

/*
 * Function name: call_event
 * Description:   cause an event to be send
 * Arguments:     event - event type to generate
 *                args - arguments to event function
 * Returns:       1 if event sending was stopped by an object, 0 else
 */
nomask varargs int
call_event(int event, mixed args...)
{
    int     i, j, r;
    string  *funcs;
    object  o, *ind;
    mapping e;

    if (!event)
	BADARG(1, call_event);
    if (!events || !(e = events[event]))
	return 0;
    ind = map_indices(e);
    for (i=sizeof(ind); i--;)
    {
	o = ind[i];
	funcs = e[o];
	for (r=0, j=sizeof(funcs); j--;)
	    r = call_other(o, funcs[j], args...);
	if (r) /* stop event sending if the previous object returned true */
	    return 1;
    }
    return 0;
}

nomask void
_F_add_event(int event, string func, object obj)
{
    if (!PRIVILEGED())
	return;
    if (!events)
	events = ([ event : ([]) ]);
    else if (!events[event])
	events[event] = ([ ]);
    
    if (!events[event][obj])
	events[event][obj] = ({ func });
    else
	events[event][obj] |= ({ func });
}

nomask void
_F_remove_event(int event, string func, object obj)
{
    if (!PRIVILEGED() || !events || !events[event] || !events[event][obj])
	return;
    events[event][obj] -= ({ func });
    if (!sizeof(events[event][obj]))
	events[event][obj] = 0;
    if (!map_sizeof(events[event]))
	events[event] = 0;
}

nomask mapping
_Q_events()
{
    return events;
}
