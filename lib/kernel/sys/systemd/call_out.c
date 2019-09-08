static mapping call_out_obj;

nomask void
add_call_out(object obj)
{
    if (!mappingp(call_out_obj))
	call_out_obj = ([ ]);
    call_out_obj[obj] = 1;
}

nomask void
list_call_outs()
{
    int    i, sz, j, sz2;
    string str, name;
    object *obj;
    mixed  *call_outs;
    
    obj = map_indices(call_out_obj);
    if (!(sz = sizeof(obj)))
    {
	this_player()->catch_tell("No call_outs pending.\n");
	return;
    }
    str = ("FUNCTION             OBJECT                               " +
	   "REMAINING TIME\n");
    for (i=0; i<sz; i++)
    {
	if (sizeof(call_outs = status(obj[i])[O_CALLOUTS]))
	{
	    name = object_name(obj[i]);
	    for (j=0, sz2=sizeof(call_outs); j<sz2; j++)
		str += sprintf("%-20s %-40s %-10d\n",
			       call_outs[j][3],
			       name, 
			       call_outs[j][2]);
	}
	else
	    call_out_obj[obj] = 0;
    }
    smore(str, this_player());
}
