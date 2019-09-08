private mapping sublocs;  /* Map of sublocations */

/*
 * Function name: init_subloc
 * Description:   initialize subloc mapping, must be called at the creation of
 *                an object that uses this support
 */
nomask void
init_subloc()
{
    sublocs = ([ ]);
}

/* 
 * Function name: add_to_subloc
 * Description:   Add an object to the given sublocation (sublocation
 *                will be created if it doesn't exist)
 * Arguments:     sloc - Name of sub location
 *                ob - object to insert at the sublocation 
 */
nomask void
add_to_subloc(string sloc, object ob)
{
    if (!sublocs[sloc])
	sublocs[sloc] = ({ ob });
    else
    {
	sublocs[sloc] -= ({ ob });
	sublocs[sloc] += ({ ob }); /* add object to end of list */
    }
}

/* 
 * Function name: remove_from_to_subloc
 * Description:   removes an object from the given sublocation
 * Arguments:     sloc - Name of sub location
 *                ob - object to insert at the sublocation 
 */
nomask void
remove_from_subloc(string sloc, object ob)
{
    if (!sublocs[sloc])
	return;
    sublocs[sloc] -= ({ ob, 0 });
}

/* 
 * Function name: query_sublocs
 * Description:   Get the current list of sublocations for this container
 * Return:        subloc list
 */
nomask string *
query_sublocs()
{
    return map_indices(sublocs);
}

/* 
 * Function name: remove_subloc
 * Description:   Remove a named sublocation of this container.
 * Arguments:     sloc - Name of sub location
 */
nomask void
remove_subloc(string sloc)
{
    sublocs[sloc] = 0;
}

/* 
 * Function name: query_subloc
 * Description:   Give the contents of a specific sublocation
 * Arguments:     sloc - sublocation
 * Returns:       array of objects within the subloc
 */
nomask object *
query_subloc(string sloc)
{
    if (!sublocs[sloc])
	return ({ });
    sublocs[sloc] -= ({ 0 });
    return sublocs[sloc];
}
