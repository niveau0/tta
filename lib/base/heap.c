inherit	"object";

#include <event.h>
#include <macros.h>
#include <move.h>
#include <properties.h>

private int     msg_size,       /* correct heap size for following message */
                heap_size,	/* Number of items in the heap */
                leave_behind,	/* how many units to leave behind if moved */
                *thread;
private string  heap_id,
                plural_long;
private object  last_split_obj, father;

static void
create_heap()
{
}

static nomask void
create_object()
{
    add_prop(IS_HEAP, 1);
    create_heap();
    thread = _DRIVER->thread();
}

/*
 * Function name: compare_thread
 * Description:   check if it is still the same thread
 * Returns:       1 if same thread, else 0
 */
nomask int
compare_thread()
{
    if (thread == _DRIVER->thread())
	return 1;
    thread = _DRIVER->thread();
    leave_behind = 0;
    msg_size = 0;
    return 0;
}

/*
 * Function name: set_plong
 * Description:   Sets the plural long description.
 * Arguments:     long - The long description
 */
nomask void
set_plong(string long)
{
    plural_long = long;
}

/*
 * Function name: query_plong
 * Description:   Gives the plural long description.
 * Returns:       string, plural long description
 */
nomask string
query_plong()
{
    return plural_long;
}

/*
 * Function name: set_heap_id
 * Description:   set a unique heap id
 * Arguments:     id - the id
 */
nomask void
set_heap_id(string id)
{
    heap_id = id;
}

/*
 * Function name: query_heap_id
 * Description:   gives the heap id modified by light and invisibility
 * Returns:       the id
 */
string
query_heap_id()
{
    int *co;

    if (!heap_id)
	return 0;
    
    return (heap_id +
	    "l" + ::query_light() +
	    "w" + ::query_weight() +
	    "v" + ::query_volume() +
	    "n" + query_prop(OBJ_NO_SHOW) +
	    "i" + query_prop(OBJ_INVIS) + "_" +
	    ((co = query_coordinates())
	     ? co[0]+"-"+co[1]+"-"+co[2] : ""));
}

/*
 * Function name: query_light
 * Description:   find out about the light
 * Returns:       total light of this heap
 */
nomask int
query_light()
{
    return ::query_light() * heap_size;
}

/*
 * Function name: query_unit_light
 * Description:   gives light of one heap unit
 * Returns:       a units light
 */
nomask int
query_unit_light()
{
    return ::query_light();
}

/*
 * Function name: query_weight
 * Description:   find out about the weight
 * Returns:       total weight of this heap
 */
nomask int
query_weight()
{
    return ::query_weight() * heap_size;
}

/*
 * Function name: query_unit_weight
 * Description:   gives weight of one heap unit
 * Returns:       a units weight
 */
nomask int
query_unit_weight()
{
    return ::query_weight();
}

/*
 * Function name: query_volume
 * Description:   find out about the volume
 * Returns:       total volume of this heap
 */
nomask int
query_volume()
{
    return ::query_volume() * heap_size;
}

/*
 * Function name: query_unit_volmue
 * Description:   gives volume of one heap unit
 * Returns:       a units volume
 */
nomask int
query_unit_volume()
{
    return ::query_volume();
}

/*
 * Function name: query_value
 * Description:   find out about the volume
 * Returns:       total value of this heap
 */
nomask int
query_value()
{
    return ::query_value() * heap_size;
}

/*
 * Function name: query_unit_value
 * Description:   gives value of one heap unit
 * Returns:       a units value
 */
nomask int
query_unit_value()
{
    return ::query_value();
}

/*
 * Function name: set_heap_size
 * Description:   sets the size of the heap
 * Arguments:     num - The new size of the heap
 */
nomask void
set_heap_size(int num)
{
    if (num <= 0)
    {
	heap_size = 0;
	/* keep object until thread finished */
	call_out("remove_object", 0);
	return;
    }
    if (E(this_object()) && num != heap_size)
	E(this_object())-> /* add/sub unitweight or volume */
	    update_internal(0, ::query_weight() * (num - heap_size),
			    ::query_volume() * (num - heap_size));
    heap_size = num;
}

/*
 * Function name: add_heap_size
 * Description:   adds some units to this heap
 * Arguments:     num - the units to add
 */
nomask void
add_heap_size(int num)
{
    set_heap_size(num + heap_size);
}

/*
 * Function name: query_heap_size
 * Description:   find out about the heap size
 * Returns:       size of this heap
 */
int
query_heap_size()
{
    return heap_size;
}

/*
 * Function name: add_msg_size
 * Description:   add some units to the moved heapsize in this thread.
 * Arguments:     num - amount to add
 */
nomask int
add_msg_size(int num)
{
    compare_thread();
    msg_size += num;
}

/*
 * Function name: query_msg_size
 * Description:   find out about the moved heapsize in this thread.
 * Returns:       size of the moved heap units
 */
nomask int
query_msg_size()
{
    compare_thread();
    return msg_size;
}

/*
 * Function name: reset_split
 * Description:   call to reset the split size so nothing will be left
 *                behind if heap is moved
 */
nomask void
reset_split()
{
    leave_behind = 0;
}

/*
 * Function name: split_heap
 * Description:   called before a pending move of a part of the heap
 *	 	  The heap size / volume is set to the part to be moved
 * Arguments:	  num: The size of the heap that is splitted from this one
 * Returns:	  (int) size of splitted heap
 */
nomask int
split_heap(int num)
{
    compare_thread();
    if (heap_size <= num)
    {
	leave_behind = 0;
	return heap_size;
    }
    leave_behind = heap_size - num;
    return num;
}

/*
 * Function name: query_split_size
 * Description:   find out about the heapsize before a pending move
 *                (after split)
 * Returns:       size of this heap
 */
nomask int
query_split_size()
{
    compare_thread();
    return heap_size - leave_behind;
}

/*
 * Function name: last_split_obj
 * Description:   if the last move of this heap has moved a split of this
 *                heap instead of this object, this function gives the object
 *                which represents the split.
 * Returns:       object, the last split of this heap that was moved, else 0
 */
nomask object
last_split_obj()
{
    return last_split_obj;
}

string
long()
{
    if (heap_size > 1)
	return plural_long;
    return ::long();
}

/*
 * Function name: config_split
 * Description:	  sets all the main vars for this heap to function.
 *		  This is called before inserting this heap into the game
 * Arguments:	  num - heap size for this heap
 *		  orig - 'Father-heap'
 *		  co - coordinates of 'Father-heap'
 */
nomask void
config_split(int num, object orig, int *co)
{
    add_msg_size(num);
    set_heap_size(num);
    set_coordinates(co);
    father = orig;
}

/*
 * Function name: heap_search_equal
 * Description:   function to find same heaps
 * Arguments:     dest - where (e.g. room object) to search for same heaps
 */
nomask object
heap_search_equal(object dest)
{
    int	   i;
    string id;
    object *ob;
    
    if (!(id = query_heap_id()))
	return 0;
    
    ob = I(dest, 6);
    for (i=sizeof(ob); i--;)
    {
	if (ob[i] != this_object() && id == ob[i]->query_heap_id())
	    return ob[i];
    }
}

/*
 * Function name: move_heap
 * Description:   Move the whole heap or a split of this to a new destination.
 * Arguments:	  dest - Object to move to
 * Returns:       (definitions see <move.h>)
 *                MOVE_OK     - Success.
 *	          MOVE_NODEST - No destination.
 *	          MOVE_NOGET  - The object can't be picked up.
 *	          MOVE_2HEAVY - The object is to heavy.
 *	          MOVE_2BIG   - The object is to big.
 *	          MOVE_NOCONT - The object is not a container.
 */
nomask int
move_heap(object dest)
{
    int    rw, rv, uw, uv, sw, sv, sz, *co;
    object heap, from;

    last_split_obj = 0;
    if (father == previous_object())
    {
	move_object(dest);
	return MOVE_OK;
    }
    
    co = query_coordinates();
    if (!dest->query_prop(IS_ROOM))
    {
	if (!dest->query_prop(IS_CONT))
	    return MOVE_NOCONT;
	if (query_prop(OBJ_NO_GET))
	    return MOVE_NOGET;
	if ((rw = dest->weight_left()) < query_weight())
	{
	    uw = query_unit_weight();
	    if (uw > rw)
		return MOVE_2HEAVY;
	    /* number of units that can be carried by 'dest' */
	    sw = rw / uw;
	}
	if ((rv = dest->volume_left()) < query_volume())
	{
	    uv = query_unit_volume();
	    if (uv > rv)
		return MOVE_2BIG;
	    /* number of units that can be carried by 'dest' */
	    sv = rv / uv;
	}
	if (sw && (sw < sv || !sv))
	    split_heap(sw);
	else if (sv)
	    split_heap(sv);
	/* coordinates are not valid if not moving 
	 * to room, needed for correct heapid
	 */
	set_coordinates(0); 
    }
    
    sz = query_split_size();
    if (heap = heap_search_equal(dest))
    {
	if (!leave_behind) /* move the whole heap */
	{
	    if ((from = E(this_object())) && living(from))
		from->remove_tool(this_object(), 0); 
	    add_msg_size(heap_size);
	    add_heap_size(heap->query_heap_size());
	    call_event(E_HEAP_MERGE, heap, heap->query_heap_size());
	    move_object(dest); /* no update_internal */
	    add_prop(OBJ_HIDE, 0); 
	    add_prop(OBJ_HIDE_PLACES, 0);
	    remove_object(heap);
	    return MOVE_OK;
	}
	last_split_obj = heap;
	heap->add_msg_size(sz);
	heap->add_heap_size(sz);
	set_heap_size(leave_behind); /* reduce this heap */
	reset_split();
	if (E(this_object())->query_prop(IS_ROOM))
	    set_coordinates(co); /* reset coordinates */
	else
	    set_coordinates(0);
	heap->call_event(E_HEAP_MERGE, this_object(), sz);
	call_event(E_MOVE, heap, E(this_object()), dest);
	call_event(E_MOVE_INV, heap, E(this_object()), dest);
	return MOVE_OK;
    }
    
    /* there is no heap at the destination so a clone is
     * needed if the heap gets splitted
     */
    if (!leave_behind) /* move the whole heap */
    {
	add_msg_size(heap_size);
	call_event(E_HEAP_MERGE, 0, heap_size);
	move_now(dest, E(this_object())); /* move _and_ update internals */
	return 0;
    }
    set_heap_size(leave_behind);
    heap = clone_object(MASTER);
    heap->config_split(sz, this_object(), query_coordinates()); /* _not_ co */
    heap->call_event(E_HEAP_MERGE, this_object(), sz);
    last_split_obj = heap;
    reset_split();
    if (E(this_object())->query_prop(IS_ROOM))
	set_coordinates(co); /* reset coordinates */
    else
	set_coordinates(0);
    return heap->move_heap(dest);
}

varargs void
remove_object(object ob)
{
    if (ob == this_object())
	reset_split();
    ::remove_object(ob);
}
