private object *attach_inv;	/* attached object */
private object *liv_inv;	/* living inventory */
private object *obj_inv;      	/* non-living inventory */
private object *heap_inv;      	/* heap inventory */

/* this object has an inventory */
nomask int
_Q_inventory()
{
    return 1;
}

/* initialize inventory */
private void
_F_init_inv()
{
    attach_inv = ({ });
    heap_inv = ({ });
    liv_inv  = ({ });
    obj_inv  = ({ });
}

/* remove an object from inventory */
nomask void
_F_rm_inv(object obj)
{
    if (!obj_inv) _F_init_inv();
    
    attach_inv -= ({ obj });
    heap_inv -= ({ obj });
    liv_inv -= ({ obj });
    obj_inv -= ({ obj });
}

/* add an object to inventory */
nomask void
_F_add_inv(object obj)
{
    if (!obj_inv) _F_init_inv();
    
    /* put new items on top of current inventory */
    if (obj->query_heap_size())
	heap_inv = ({ obj }) + heap_inv;
    else if (obj->_Q_alive())
	liv_inv = ({ obj }) + liv_inv;
    else
	obj_inv = ({ obj }) + obj_inv;
}

nomask void
_F_attach(object obj)
{
    if (previous_program() != "/base/object")
	error("Illegal call to _F_attach");
    
    if (!obj_inv) _F_init_inv();
    
    attach_inv += ({ obj });
}

/*
 * Function name: attached
 * Description:   return an array with the attachments of this object  
 * Returns:       array
 */
nomask varargs object *
attached()
{
    if (!obj_inv) _F_init_inv();
    
    return attach_inv[..];
}

/*
 * Function name: room_inventory
 * Description:   return the inventory of the room
 * Arguments:     flag - see 'inventory' manual
 * Returns:       array
 */
private object *
room_inventory(int flag)
{
    int     i;
    object  *e, *inv;
    mapping extra;

    if (!mappingp(extra = this_object()->query_loaded_exit()))
	return ({ });
    
    inv = ({ });
    e = map_indices(extra);
    i = sizeof(e);
    switch(flag)
    {
      case 3:
	  while(i--) inv += e[i]->inventory(6);
	  return heap_inv + inv;
      case 2:
	  while(i--) inv += e[i]->inventory(5);
	  return liv_inv + inv;
      case 1:
	  while(i--) inv += e[i]->inventory(4);
	  return obj_inv + heap_inv + inv;
      default:
	  while(i--) inv += e[i]->inventory(7);
	  return liv_inv + obj_inv + heap_inv + inv;
    }
}

/*
 * Function name: inventory
 * Description:   return an array with the inventory of this object  
 * Arguments:     flag - 0 whole inventory,
 *                       1 only items,
 *                       2 only livings,
 *                       3 only heaps
 *                       4 only items (without surrounding rooms),
 *                       5 only livings (without surrounding rooms),
 *                       6 only heaps (without surrounding rooms)
 *                       7 whole inventory (without surrounding rooms)
 * Returns:       array
 */
nomask varargs object *
inventory(int flag)
{
    if (!obj_inv) _F_init_inv();
    
    if (flag < 4 && function_object("query_loaded_exit", this_object()))
	return room_inventory(flag);
    switch(flag)
    {
      case 6:
      case 3:
	  return heap_inv[..];
      case 5:
      case 2:
	  return liv_inv[..];
      case 4:
      case 1:
	  return obj_inv + heap_inv;
    }
    return liv_inv + obj_inv + heap_inv;
}

/*
 * Function name: deep_inventory
 * Description:   return the recursive inventory of an object  
 * Arguments:     obj - object to query
 *                flag - see manual for 'inventory'
 * Returns:       array, inventory
 */
nomask varargs object *
deep_inventory(int flag)
{
    int    i;
    object *inv, *sub;

    inv = inventory(flag);
    for (i=sizeof(inv); i--;)
    {
	sub = inv[i]->inventory(flag);
	if (sizeof(sub))
	{
	    inv = sub + inv;
	    i += sizeof(sub);
	}
    }
    return inv;
}
