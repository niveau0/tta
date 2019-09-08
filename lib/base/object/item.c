mapping pseudo_items;

/*
 * Function name: add_item 
 * Description:   add a pseudo item 
 * Arguments:     n - item names
 *                pn - item plural names
 *                desc - item description
 *                id - item id, the number of the item
 *                co - optional coordinates for special locations
 *                     (e.g. smaller exits within a room)
 */
nomask varargs void
add_item(string *n, string *pn, string desc, int id, int *co)
{
    if (!pseudo_items)
	pseudo_items = ([ ]);
    if (id < 1)
	error("Item id out of range");
    if (!function_object("query_room_file", this_object()) || !co)
	pseudo_items[id] = ({ n, pn, desc });
    else
	pseudo_items[id] = ({ n, pn, desc, co });
}

/*
 * Function name: remove_item 
 * Description:   remove a pseudo item (you may only remove the last added)
 * Arguments:     id - item id
 */
nomask void
remove_item(int id)
{
    if (!pseudo_items)
	return;
    if (id < 1)
	error("Item id out of range");
    pseudo_items[id] = 0;
}

/*
 * Function name: query_item
 * Description:   query item data for a given item id, -1 gives the whole item
 *                mapping
 * Arguments:     id - item id
 * Returns:       array, ({ names, plural names, description, coordinates })
 *                notify that the coordinates field is optional
 */
nomask mixed
query_item(int id)
{
    if (!pseudo_items)
    {
	if (id == -1)
	    return ([ ]);
	return 0;
    }
    if (id == -1)
	return pseudo_items;
    return pseudo_items[id];
}

/*
 * Function name: parse_item
 * Description:   called by cmdsoul to find all matching pseudo items
 *                of this object
 * Arguments:     cmd - the command string to check
 *                type - match type, 1 singular, 2 plural, 3 sing. or plural
 * Returns:       array, ({ descriptions of matching items })
 */
nomask string **
parse_item(string cmd, int type)
{
    int    i, *ind;
    string **found;
    mixed  current;

    if (!pseudo_items)
	return ({ });

    ind = map_indices(pseudo_items);
    for (i=sizeof(ind), found=allocate(i); i--;)
    {
	current = pseudo_items[ind[i]];
	if (type == 2)
	{
	    if (IN_ARRAY(cmd, current[1]))
	    {
		found[i] = current[..];
		found[i][2] = process_value(current[2]);
	    }
	    continue;
	}
	if (type == 1)
	{
	    if (IN_ARRAY(cmd, current[0]))
	    {
		found[i] = current[..];
		found[i][2] = process_value(current[2]);
	    }
	    continue;
	}
	if (IN_ARRAY(cmd, current[1]))
	{
	    found[i] = current[..];
	    found[i][2] = process_value(current[2]);
	}
	else if (IN_ARRAY(cmd, current[0]))
	{
	    found[i] = current[..];
	    found[i][2] = process_value(current[2]);
	}
    }
    return found - ({ 0 });
}
