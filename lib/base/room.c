inherit "/system/inventory";
inherit "object/props";

#include <config.h>
#include <event.h>
#include <filepath.h>
#include <macros.h>
#include <math.h>
#include <mudtime.h>
#include <properties.h>
#include <room.h>

nomask void save();
nomask int *query_room_size();
nomask int calc_z_coordinate(int *co);

private int     room_cur_light; /* Current lightvalue in the room */

static string        room_file; /* .o file to load */
static object          *clones; /* existing objects in room */
static mapping           exits; /* loaded exits */

int                      light,	/* lightvalue of the room itself */
                        height, /* height from floor to roof */
			 xsize, /* x-size of room in inch */
			 ysize, /* y-size of room in inch */
		       climate, /* room climate */
		          type, /* room type */
                    *edge_base, /* z-coordinate of each boundary n, s, w, e */
		  *room_coords; /* world coordinates of this room */
string              room_short,	/* Short description */
                     room_long, /* Long description */
                   parent_room; /* wilderness parent room */
mapping             room_exits;	/* exit data */
mixed            objs_to_clone; /* objects that should be created in room */

#include "object/item.c"

/*
 * Default constructor
 */
static void
create_room()
{
}

static nomask void
create()
{
    exits        = ([ ]);
    room_exits   = ([ ]);
    edge_base    = ({ 0, 0, 0, 0 });
    room_coords  = ({ 0, 0, 0 });
    
    init_props();
    create_room();
    add_prop(IS_ROOM, 1);	/* This is a room */
}

/*
 * Function name: E_room_move
 * Description:   event: notice movement in room
 * Arguments:     ob - the moving object
 *                from - from where it comes
 *                to - where it goes
 */
nomask void
E_room_move(object ob, object from, object to)
{
    int *co;
    
    if (!ob || from == to || !(co = ob->query_coordinates()))
	return;
    co[2] = calc_z_coordinate(co);
    ob->set_coordinates(co);
}

/*
 * Function name: set_exit_status
 * Description:   change the exit flag for open or closed exit
 * Arguments:     exit - pathname of the exit
 *                s - new status, see query_exits
 */
nomask void
set_exit_status(string exit, int s)
{
    if (!sizeof(room_exits[exit]))
	return;
    room_exits[exit][EXIT_STATUS] = s;
    room_exits[exit][EXIT_OBJ] = _ROOMD->
				 load_room(FPATH(room_file + "/..",
						 exit));
}

/*
 * Function name: add_exit
 * Description:   add an exit  
 * Arguments:     file - the destination room
 *                data - exit data array
 */
nomask void
add_exit(string file, mixed data)
{
    if (previous_program() != CREATE_ROOM)
	error("Illegal call to add_exit");
    room_exits[file] = data;
    save();
}

/*
 * Function name: query_exits
 * Description:   Gives a list of the possible exits from this room.
 * Returns:       ([ roomfile : ({ direction, xoffset, yoffset,
 *                            width, height, status, object }), 
 *                   ... ])
 *                where 'direction' is the direction code, like 'w' for 'west'
 *                and 'status' is the status of the exit (see room.h)
 */
nomask mapping
query_exits()
{
    return room_exits; 
}

/*
 * Function name: query_loaded_exit
 * Description:   Gives a list of the connected exitrooms
 * Returns:       mapping ([ roomobject : roompath ])
 */
nomask mapping 
query_loaded_exit()
{
    int    i;
    string *ind;
    object ob;
    mixed  exit;
	
    ind = map_indices(room_exits);
    for (i=sizeof(ind); i--;)
    {
	exit = room_exits[ind[i]];
	if (exit[EXIT_OBJ])
	    continue;
	if (ob = _ROOMD->load_room(FPATH(room_file + "/..", ind[i])))
	{
	    exits[ob] = ind[i];
	    room_exits[ind[i]][EXIT_OBJ] = ob;
	}
    }
    return exits;
}

/*
 * Function name: exit_visible
 * Description:   check if an object is visible through an exit from 
 *                within this room.
 * Arguments:     p0 - viewer position (inside this room)
 *                p1 - object position
 *                env - object environment
 * Returns:       1 if object is visible, else 0
 */
nomask int
exit_visible(int *p0, int *p1, object env)
{
    int     i, sz, co0, co1, rc0, rc1, ix, iy, iz, sect;
    string  *ind;
    mixed   exit;

    if (!p0 || !p1 || !env ||
	env == this_object() || !env->query_prop(IS_ROOM))
	return 1;
    ind = map_indices(room_exits);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if ((exit = room_exits[ind[i]])[EXIT_OBJ] != env)
	    continue;
	if (exit[EXIT_STATUS] != EXIT_OPEN)
	    return 0;
	switch (exit[EXIT_DIR])
	{
	  case 'u': /* ceiling */
	      ix = 0; iy = 1; iz = 2;
	      sect = room_coords[iz]+height;
	      break;
	  case 'd': /* floor */
	      ix = 0; iy = 1; iz = 2;
	      sect = room_coords[iz];
	      break;
	  case 'n': /* north wall */
	      ix = 0; iy = 2; iz = 1;
	      sect = room_coords[iz]+ysize;
	      break;
	  case 's': /* south wall */
	      ix = 0; iy = 2; iz = 1;
	      sect = room_coords[iz];
	      break;
	  case 'e': /* east wall */
	      ix = 1; iy = 2; iz = 0;
	      sect = room_coords[iz]+xsize;
	      break;
	  case 'w': /* west wall */
	      ix = 1; iy = 2; iz = 0;
	      sect = room_coords[iz];
	      break;
	}
	if (p0[iz] == p1[iz])
	    continue;
	
	rc0 = room_coords[ix];
	rc1 = room_coords[iy];

	if (p0[ix] == p1[ix])
	    co0 = p0[ix];
	else
	    co0 = LINE(sect, p0[iz], p0[ix], p1[iz], p1[ix]);
	
	if (p0[iy] == p1[iy])
	    co1 = p0[iy];
	else
	    co1 = LINE(sect, p0[iz], p0[iy], p1[iz], p1[iy]);
	
	if (co0 < rc0 + exit[EXIT_OFFX] ||
	    co0 >= rc0 + exit[EXIT_OFFX] + exit[EXIT_WIDTH] ||
	    co1 < rc1 + exit[EXIT_OFFY] ||	
	    co1 >= rc1 + exit[EXIT_OFFY] + exit[EXIT_HEIGHT])
	    /* outside exit boundaries? */
	    continue;
	return 1;
    }
    return 0;
}

/*
 * Function name: search_exit
 * Description:   check if an exit is passed
 * Arguments:     dir - direction
 *                co - new object coordinates
 * Returns:       object, exit room if found, else 0
 */
nomask object
search_exit(int dir, int *co)
{
    int    i, co0, co1, rc0, rc1;
    string *ind;
    object ob;
    mixed  exit;

    switch (dir)
    {
      case 'u': /* ceiling */
      case 'd': /* floor */
	  rc0 = room_coords[0]; rc1 = room_coords[1];
	  co0 = co[0]; co1 = co[1];
	  break;
      case 'e': /* passing eastern wall */
	  rc1 = calc_z_coordinate(({ room_coords[0] + xsize, co[1] }));
	  rc0 = room_coords[1];
	  co0 = co[1]; co1 = co[2];
	  break;
      case 'w': /* passing western wall */
	  rc1 = calc_z_coordinate(({ room_coords[0], co[1] }));
	  rc0 = room_coords[1];
	  co0 = co[1]; co1 = co[2];
	  break;
      case 'n': /* passing northern wall */
	  rc1 = calc_z_coordinate(({ co[0], room_coords[1] + ysize }));
	  rc0 = room_coords[0];
	  co0 = co[0]; co1 = co[2];
	  break;
      case 's': /* passing southern wall */
	  rc1 = calc_z_coordinate(({ co[0], room_coords[1] }));
	  rc0 = room_coords[0];
	  co0 = co[0]; co1 = co[2];
	  break;
    }
    ind = map_indices(room_exits);
    for (i=sizeof(ind); i--;)
    {
	if ((exit = room_exits[ind[i]])[EXIT_DIR] != dir)
	    continue;
	if (exit[EXIT_STATUS] == EXIT_CLOSED)
	    continue;
	if (co0 < rc0 + exit[EXIT_OFFX] ||
	    co0 >= rc0 + exit[EXIT_OFFX] + exit[EXIT_WIDTH] ||
	    co1 < rc1 + exit[EXIT_OFFY] ||	
	    co1 >= rc1 + exit[EXIT_OFFY] + exit[EXIT_HEIGHT])
	    /* outside exit boundaries? */
	    continue;
	if (!(ob = exit[EXIT_OBJ]))
	    /* exit room loaded? */
	    return 0;
	if (in_range(co, ob->query_coordinates(), ob->query_room_size()))
	    /* object coordinates inside exitroom-coordinates? */
	    return ob;
	return 0;
    }
    return 0;
}

/*
 * Function name: determine_border_coords
 * Description:   determine the nearest border-coordinates for the
 *                given arguments
 * Arguments:     oco - 'to come from' coordinates
 *                co - 'to go to' coordinates
 *                dir - direction to check
 * Returns:       new coordinates, 0 if not possible or unchanged
 */
nomask int *
determine_border_coords(int *oco, int *co, string dir)
{
    oco = oco[..];
    switch (dir)
    {
      case "north":
	  if (oco[1] >= room_coords[1] + ysize - 1)
	      return 0;
	  oco[1] = room_coords[1]+ysize-1;
	  break;
      case "south":
	  if (oco[1] <= room_coords[1])
	      return 0;
	  oco[1] = room_coords[1];
	  break;
      case "east":
	  if (oco[0] >= room_coords[0] + xsize - 1)
	      return 0;
	  oco[0] = room_coords[0]+xsize-1;
	  break;
      case "west":
	  if (oco[0] <= room_coords[0])
	      return 0;
	  oco[0] = room_coords[0];
	  break;
      case "northeast":
	  if (oco[1] >= room_coords[1] + ysize - 1 ||
	      oco[0] >= room_coords[0] + xsize - 1)
	      return 0;
	  /* check which border is nearest */
	  if (co[1] >= room_coords[1] + ysize - 1)
	      oco[1] = room_coords[1] + ysize - 1;
	  if (co[0] >= room_coords[0] + xsize - 1)
	      oco[0] = room_coords[0] + xsize - 1;
	  break;
      case "northwest":
	  if (oco[1] >= room_coords[1] + ysize - 1 ||
	      oco[0] <= room_coords[0])
	      return 0;
	  /* check which border is nearest */
	  if (co[1] >= room_coords[1] + ysize - 1)
	      oco[1] = room_coords[1] + ysize - 1;
	  if (co[0] <= room_coords[0])
	      oco[0] = room_coords[0];
	  break;
      case "southeast":
	  if (oco[1] <= room_coords[1] ||
	      oco[0] >= room_coords[0] + xsize - 1)
	      return 0;
	  /* check which border is nearest */
	  if (co[1] <= room_coords[1])
	      oco[1] = room_coords[1];
	  if (co[0] >= room_coords[0] + xsize - 1)
	      oco[0] = room_coords[0] + xsize - 1;
	  break;
      case "southwest":
	  if (oco[1] <= room_coords[1] || oco[0] <= room_coords[0])
	      return 0;
	  /* check which border is nearest */
	  if (co[1] <= room_coords[1])
	      oco[1] = room_coords[1];
	  if (co[0] <= room_coords[0])
	      oco[0] = room_coords[0];
	  break;
      default:
	  return 0;
    }
    return oco;
}

/*
 * Function name: query_leaving_room
 * Description:   decide if new coordinates are within this room
 * Arguments:     co - coordinates to test
 * Returns:       new room object for leaving,
 *                this room object if not leaving,
 *                0 if no exit found
 */
nomask object
query_leaving_room(int *co)
{
    object ob;
    
    if (co[2] < room_coords[2])
	return search_exit('d', co); /* down */
    if (co[2] > room_coords[2] + height)
	return search_exit('u', co); /* up */

    if (co[1] < room_coords[1])
    {
	if (ob = search_exit('s', co)) /* south */
	    return ob;
	if (co[0] < room_coords[0])
	    return search_exit('w', co); /* west */
	if (co[0] >= room_coords[0] + xsize)
	    return search_exit('e', co); /* east */
	return 0;
    }
    if (co[1] >= room_coords[1] + ysize)
    {
	if (ob = search_exit('n', co)) /* north */
	    return ob;
	if (co[0] < room_coords[0])
	    return search_exit('w', co); /* west */
	if (co[0] >= room_coords[0] + xsize)
	    return search_exit('e', co); /* east */
	return 0;
    }
    if (co[0] < room_coords[0])
	return search_exit('w', co); /* west */
    if (co[0] >= room_coords[0] + xsize)
	return search_exit('e', co); /* east */
    return this_object();
}

/*
 * Function name: query_light
 * Description:   Returns the light status in this room
 * Arguments:     oblight - optional light value to subtract
 * Returns:	  Light value
 */
nomask varargs int
query_light(int oblight)
{
    oblight = (room_cur_light - oblight) * 20 / 100 + light; 
    if (type & TYPE_INSIDE)
	return oblight;
    if (NIGHT)
 	return oblight + NIGHTLIGHT;
    return oblight + DAYLIGHT;
}
    
/* 
 * Function name: update_internal
 * Description:   updates the internal light
 * Arguments:     l - light difference
 */
nomask void
update_internal(int l)
{
    room_cur_light += l;
}

/*
 * Function name: long
 * Description:   describe the room 
 * Returns:       string holding the long description of the room.
 */
string
long()
{
    return room_long;
}

/*
 * Function name: short
 * Description:   describe the room briefly
 * Returns:       string holding the short description of the room.
 */
nomask string 
short()
{
    return room_short;
}

/*
 * Function name: remove_object
 * Description:   Removes this object from the game.
 */
varargs void
remove_object(object ob)
{
    if (!ob || ob == this_object())
	_ROOMD->remove_room(room_file);
    ::remove_object(ob);
}

/*
 * Function name: query_coordinates
 * Description:   gives the coordinates of this room
 * Returns:       array with x, y, z coordinate
 */
nomask int *
query_coordinates()
{
    return room_coords;
}

/*
 * Function name: query_height
 * Description:   return height of the room
 * Returns:       integer
 */
nomask int
query_height()
{
    return height;
}

/*
 * Function name: calc_z_coordinate
 * Description:   calculate the valid z value for given x and y coordinates
 * Arguments:     co - the coordinates of an object
 * Returns:       integer, the z coordinate of that object
 */
nomask int
calc_z_coordinate(int *co)
{
    int hx, hy, dx, dy;

    if (!ysize || !xsize)
	return 0;
    
    dy = edge_base[ROOM_EDGE_S] - edge_base[ROOM_EDGE_N];
    hy = edge_base[ROOM_EDGE_S] - dy * (co[1] - room_coords[1]) / ysize;
    dx = edge_base[ROOM_EDGE_W] - edge_base[ROOM_EDGE_E];
    hx = edge_base[ROOM_EDGE_W] - dx * (co[0] - room_coords[0]) / xsize;
    
    return (hx + hy) / 2 + room_coords[2];
}

/*
 * Function name: query_edge_base
 * Description:   return z-position of room edge
 * Arguments:     edge, see definitions within <room.h>
 * Returns:       integer or array (if querying all edges)
 */
nomask mixed
query_edge_base(int edge)
{
    if (edge == ROOM_EDGE_ALL)
	return edge_base;
    return edge_base[edge];
}

/*
 * Function name: query_room_size
 * Description:   return size of this room
 * Returns:       ({ xsize, ysize, height })
 */
nomask int *
query_room_size()
{
    return ({ xsize, ysize, height });
}

/*
 * Function name: query_climate
 * Description:   return climate of this room
 * Returns:       int
 */
nomask int
query_climate()
{
    return climate;
}

/*
 * Function name: query_type
 * Description:   return type of this room
 * Returns:       int
 */
nomask int
query_type()
{
    return type;
}

/*
 * Function name: query_move_fatigue
 * Description:   gives fatigue value when moving in this room
 * Returns:       integer, default 1
 */
int
query_move_fatigue()
{
    return 1;
}

/*
 * Function name: collision
 * Description:   check if some object is blocking someones way
 * Arguments:     co - coordinates of moving object
 *                nco - coordinates where object wants to be after move
 * Returns:       object, the blocker else 0
 */
nomask object
collision(int *co, int *nco)
{
    int    i, point, bx, by, bxmax, bymax, dx, dy, dxdy, dydx,
	   xco, yco, xnco, ynco, *bco, *range;
    object ob, *obj;

    obj = query_event(E_BLOCK);
    if (!(i = sizeof(obj)))
	return 0;

    xco = co[0];
    yco = co[1];
    xnco = nco[0];
    ynco = nco[1];
    dx = xnco - xco;
    dy = ynco - yco;
    if (!dx && !dy) /* just to be sure we really move */
	return 0;
    if (dx)
	dydx = dy / dx;
    if (dy) 
	dxdy = dx / dy;
    
    while (i--)
    {
	ob = obj[i];
	if (!(range = ob->query_range())) 
	    continue;
	bco = ob->query_coordinates();
	if (in_range(nco, bco, range))
	    return ob;
	bx = bco[0];
	by = bco[1];
	bxmax = bx + range[0];
	bymax = by + range[1];

	if (!dy) /* moving east-west */
	{
	    if (by <= yco && bymax >= yco &&
		((bx >= xco && bx <= xnco) ||
		 (bx >= xnco && bx <= xco) ||
		 (bxmax >= xco && bxmax <= xnco) ||
		 (bxmax >= xnco && bxmax <= xco)))
		return ob;
	    continue;
	}
	if (!dx) /* moving north-south */
	{
	    if (bx <= xco && bxmax >= xco &&
		((by >= yco && by <= ynco) ||
		 (by >= ynco && by <= yco) ||
		 (bymax >= yco && bymax <= ynco) ||
		 (bymax >= ynco && bymax <= yco)))
		return ob;
	    continue;
	}
	/* moving diagonal */
	if ((bx >= xco && bx <= xnco) ||
	    (bx >= xnco && bx <= xco))
	{
	    point = dydx * (bx - xco) + yco;
	    if (point >= by && point <= bymax)
		return ob;
	}
	if ((bxmax >= xco && bxmax <= xnco) ||
	    (bxmax >= xnco && bxmax <= xco))
	{
	    point = dydx * (bxmax - xco) + yco;
	    if (point >= by && point <= bymax)
		return ob;
	}
	if ((bymax >= yco && bymax <= ynco) ||
	    (bymax >= ynco && bymax <= yco))
	{
	    point = dxdy * (bymax - yco) + xco;
	    if (point >= bx && point <= bxmax)
		return ob;
	}
    }
    return 0;
}

/*
 * Function name: load
 * Description:   load the room
 */
void
load()
{
    int i;
    
    if (!room_file)
	return;
    restore_object(room_file);
    if (sizeof(edge_base) != 4)
	edge_base = ({ room_coords[2], room_coords[2],
		       room_coords[2], room_coords[2] });
    if (!mappingp(exits))
	exits = ([ ]);
    if (!mappingp(room_exits))
	room_exits = ([ ]);
    else
    {
	int    i, j;
	mixed  *ind, exit;
	
	ind = map_indices(room_exits);
	if ((j=sizeof(ind)) && !stringp(ind[0]))
	{
	    while (j--)
	    {
		exit = room_exits[ind[j]];
		for (i=sizeof(exit)-6; i>=0; i-=6)
		{		
		    room_exits[exit[i]] = ({ ind[j], exit[i+1], exit[i+2],
					     exit[i+3], exit[i+4], exit[i+5],
					     0 });
		}
		room_exits[ind[j]] = 0;
	    }
	    call_out("save", 0);
	}
    }

    add_event(E_MOVE_INV, "E_room_move", this_object());
    
    if (i = sizeof(objs_to_clone))
    {
	if (sizeof(clones) != i)
	    clones = allocate(i);
	while (i--)
	{
	    if (clones[i])
		continue;
	    if (!catch(clones[i] = clone_object(objs_to_clone[i][0])))
	    {
		clones[i]->init_reload(objs_to_clone[i][1]);
		clones[i]->move(this_object());
	    }
	}
    }
}

/*
 * Function name: save
 * Description:   save the room
 */
nomask void
save()
{
    int    i, sz;
    object *inv;
    
    if (!room_file)
	error("No room path");
    inv = I(this_object(), 7);
    sz  = sizeof(inv);
    objs_to_clone = allocate(sz);
    for (i=0; i<sz; i++)
    {
	if (living(inv[i]) && !inv[i]->query_npc())
	    continue;
	objs_to_clone[i] = ({ MASTER_OB(inv[i]),
			      inv[i]->reload_data() });
    }
    objs_to_clone -= ({ 0 });
    if (!sizeof(objs_to_clone))
	objs_to_clone = 0;
    save_object(room_file);
}

/*
 * Function name: set_room_file
 * Description:   set the path of the .o file
 * Arguments:     f - filename
 */
nomask string
set_room_file(string f)
{
    room_file = f;
}

/*
 * Function name: query_room_file
 * Description:   gives related room file
 * Returns:       file path
 */
nomask string
query_room_file()
{
    return room_file;
}
