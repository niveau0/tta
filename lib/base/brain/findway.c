/* monster brain
 * for finding a way to a target in this room,
 * avoiding blocking objects.
 * Advice: run init_brain before doing anything else.
 */

/* todo: implement "query_version" in room */

#include <macros.h>
#include "../living/prototypes.h"
#include <event.h>

private int    *dx, *dy;
private int    ***map, **togo, togostep, togosz, *x, *y;
private int    *target_field, known_version;
private string *ds, *dslong;

object approach_obj;

int query_step();


/*
 * Function name: init_brain
 * Description:   initialise basic brain functions.
 */
nomask void
init_brain()
{
    dx = ({  0,  1,  1,  1,  0, -1, -1, -1 });
    dy = ({ -1, -1,  0,  1,  1,  1,  0, -1 });
    ds = ({ "s", "se", "e", "ne", "n", "nw", "w", "sw"});
    dslong = ({ "south", "southeast", "east",
		"northeast", "north", "northwest",
		"west", "southwest"});

    approach_obj=this_object();
    target_field=({ });
}


static int
sort_int(int a, int b)
{
    return a > b;
}

/*
 * Function name: brain_find_way
 * Description:   Find a way to ob and start approaching it.
 * Arguments:     ob - object to approach.
 * Returns:       -1 if ob can't be reached,
 *                or the number of "stations" in the path.
 */
private int
brain_find_way(object ob)
{
    object room, *blockers, me;
    int sz, i, j, k, l, xsz, ysz, maximum, dist;
    int mei, mej, obi, obj;
    int *me_co, *ob_co, *range, *co, *sco;
    int **todo, t, **newtodo, nt, **swap;
    mapping edge_x, edge_y;
    string out;
    
    /* me and ob are in different rooms? */
    if ((room=E(me=this_object()))
	!= E(ob))
	return -1;

    me_co = query_coordinates();
    ob_co = ob->query_coordinates();
    if (MAX(ABS(me_co[1] - ob_co[1]), ABS(me_co[0] - ob_co[0]))
	< query_step())
    {
	direct_approach(this_object(), ob);
	togosz=0;
	approach_obj=this_object();
	target_field=({ });

	return 0;
    }

    /* are there any blockers at all? */
    if (!(sz = sizeof(blockers = map_indices(room->query_event(E_BLOCK)))))
    {
	togosz=0;
	approach_obj=ob;
	target_field=({ });

	return 0;
    }

    /* init: room boundary is primary boundary for mesh array */
    edge_x = ([ (co = room->query_coordinates())[0] : 1,
	      co[0] + (sco = room->query_room_size())[0] : 1  ]);
    edge_y = ([ co[1] : 1, co[1] + sco[1] : 1 ]);

    maximum = sco[0] + sco[1];


    /* look for more boundaries */
    for (i=0; i<sz; i++)
    {
	edge_x[ (co = blockers[i]->query_coordinates())[0] ] = 1;
	edge_x[ co[0] +
	      (sco = blockers[i]->query_range())[0] ] = 1;
	edge_y[ co[1] ] = 1;
	edge_y[ co[1] + sco[1] ] = 1;
    }

    x = map_indices(edge_x);
    y = map_indices(edge_y);

    /* maybe one obj fills the room? */
    if ((xsz = (sizeof(x) - 1)) == 1 || (ysz = (sizeof(y) - 1)) == 1)
	return -1;

    sort_array(x, "sort_int", me);
    sort_array(y, "sort_int", me);
    for (mei=xsz-1; x[mei]>me_co[0]; mei--);
    for (mej=ysz-1; y[mej]>me_co[1]; mej--);
    for (obi=xsz-1; x[obi]>ob_co[0]; obi--);
    for (obj=ysz-1; y[obj]>ob_co[1]; obj--);
    target_field=({obi, obj});

    /* Target in next field? */
    if (ABS(obi-mei)<2 && ABS(obj-mej)<2)
    {
	togosz=0;
	approach_obj=ob;

	return 0;
    }

    todo = allocate(xsz*ysz);
    newtodo = allocate(xsz*ysz);

    /* now fill in size parameters */
    map = allocate( xsz );
    sco = ({ 0, 0 });
    co = ({ 0, 0 });
    for (i=0; i<xsz; i++)
    {
	map[i] = allocate( ysz );
	sco[0] = (x[i+1] - (co[0] = x[i])) / 2;
	co[0] += sco[0];
	for (j=0; j<ysz; j++)
	{
	    sco[1] = (y[j+1] - (co[1] = y[j])) / 2;
	    co[1] += sco[1];
	    map[i][j] = ({ maximum, MAX( sco[0], sco[1]) });
	    for (k=0; k<sz; k++)
	    {
		ob_co = blockers[k]->query_coordinates();
		range = blockers[k]->query_range();

		if (ob_co[0] < co[0]
		    && ob_co[1] < co[1]
		    && ob_co[0] + range[0] > co[0]
		    && ob_co[1] + range[1] > co[1])
		{
		    map[i][j][0] = -1;
		    k=sz;
		}
	    }
	}
    }

    /* mark target */
    co = ({ 0, 0 });
    sco = ob->query_coordinates();

    map[i=obi][j=obj][0] = 0;
    nt=0;
    for (k=0; k<8; k++)
    {
	co[0]=i + dx[k];
	co[1]=j + dy[k];
	if (co[0] >= 0 && co[0] < xsz
	    && co[1] >= 0 && co[1] < ysz
	    && (map[ co[0] ][ co[1] ][0] > 0))
	{
	    map[ co[0] ][ co[1] ][0]
		= MAX(ABS((x[co[0]]+x[co[0]+1])/2-sco[0]),
		      ABS((y[co[1]]+y[co[1]+1])/2-sco[1]));
	    newtodo[nt++] = ({ co[0], co[1] });
	}
    }
    swap = todo; todo = newtodo; newtodo = swap;
    
    while ( (t = nt) )
    {
	nt = 0;
	for (l=0; l<t; l++)
	{
	    i = todo[l][0];
	    j = todo[l][1];
	    for (k=0; k<8; k++)
	    {
		co[0]=i + dx[k];
		co[1]=j + dy[k];
		if (co[0] >= 0 && co[0] < xsz
		    && co[1] >= 0 && co[1] < ysz
		    && ((dist = (map[i][j][1] + map[i][j][0]
				 + map[ co[0] ][ co[1] ][1]))
			< map[ co[0] ][ co[1] ][0]))
		{
		    map[ co[0] ][ co[1] ][0] = dist;
		    newtodo[nt++] = ({ co[0], co[1] });
		}
	    }
	}
	swap = todo; todo = newtodo; newtodo = swap;
    }

    /* now look for the way */
    i=mei; j=mej;
    sco = ({ 0, 0 });
    co = ({ 0, 0 });
    t=0;
    
    sco = ({ -1, maximum});
    for (k=0; k<8; k++)
    {
	co[0]=i + dx[k];
	co[1]=j + dy[k];
	if (co[0] >= 0 && co[0] < xsz
	    && co[1] >= 0 && co[1] < ysz
	    && map[ co[0] ][ co[1] ][0] >= 0
	    && (dist =
		(MAX(ABS((x[co[0]]+x[co[0]+1])/2-me_co[0]),
		     ABS((y[co[1]]+y[co[1]+1])/2-me_co[1])))
		+ map[ co[0] ][ co[1] ][0]) < sco[1])
	    sco = ({ k, dist });	    
    }
    map[i][j][0]=sco[1];
    i += dx[sco[0]];
    j += dy[sco[0]];

    while (map[i][j][0])
    {
	todo[t++] = ({ (x[i] + x[i+1]) / 2, (y[j] + y[j+1]) / 2, i,j});

	sco = ({ -1, map[i][j][0]});
	for (k=0; k<8; k++)
	{
	    co[0]=i + dx[k];
	    co[1]=j + dy[k];
	    if (co[0] >= 0 && co[0] < xsz
		&& co[1] >= 0 && co[1] < ysz
		&& map[ co[0] ][ co[1] ][0] >= 0
		&& map[ co[0] ][ co[1] ][0] < sco[1])
		sco = ({ k, map[ co[0] ][ co[1] ][0] });	    
	}
	if (sco[0] < 0)
	    return 42;

	i += dx[sco[0]];
	j += dy[sco[0]];
    }

    togo=allocate(t);
    
    togo[0]=todo[0];
    for (i=1; i<t; i++)
	togo[i]=todo[i];
    
    togostep=0;
    togosz=t;
    approach_obj=ob;

    return togosz;
}


/*
 * Function name: brain_approach
 * Description:   actually approach coordinates.
 *                Possibly call brain_find_way() to compute a new path.
 *                if something blocks, try next directions.
 * Arguments:     co - an array of at lest two elements,
 *                	coordinates in inch.
 * Returns:       -1 on failure, i.e. the target isn't reachable,
 *                	for whatever reasons, or is in direct reach;
 *                -2 if something weird happened;
 *                1 on success, i.e., we made one step in the right
 *                	direction;
 *                2 if we made a step in the right direction, but only
 *                	after recalculating the path.
 *                3 if we made a step in the right direction and now
 *                	the target is in reach.
 */
nomask int
brain_approach(object target)
{
    int *me_co, *ob_co, d, d2, step, result;
    object me, env;
    string dir;

    result=1;

    if (target == (me=this_object()) || !living(target))
	return -1;

    env=E(me);

    if (target != approach_obj
	|| (sizeof(target_field) &&
	    ((ob_co = target->query_coordinates())[0] < x[target_field[0]]
	     || ob_co[1] < y[target_field[1]]
	     || ob_co[0] > x[target_field[0]+1]
	     || ob_co[1] > y[target_field[1]+1]))
	|| env != E(target)
	|| env->query_version() != known_version)
	if (brain_find_way(target) < 0)
	    return -1;
	else
	    result=2;
    
    me_co=query_coordinates();

    if (togosz)
    {
	if ( me_co[0]>=x[togo[togostep][2]] &&
	     me_co[1]>=y[togo[togostep][3]] &&
	     me_co[0]<=x[togo[togostep][2]+1] &&
	     me_co[1]<=y[togo[togostep][3]+1] )
	    togostep++;
	
	if (togostep==togosz)
	{
	    togosz=0;
	    ob_co=({ });
	}
	else
	    ob_co=({togo[togostep][0], togo[togostep][1]});
    }

    if (!togosz)
	ob_co=target->query_coordinates();
    
    if (!sizeof(ob_co))
	return -2;
    
    dir="";
    step=query_step();
    
    if (MAX(ABS(d = me_co[1] - ob_co[1]),
	    ABS(d2 = me_co[0] - ob_co[0])) < step)
    {
	if (d < 0)
	    dir = "n";
	else if (d > 0)
	    dir = "s";
	
	if (d2 < 0)
	    dir += "e";
	else if (d2 > 0)
	    dir += "w";

	d=member_array(dir, ds);
	
	if (togosz)
	{
	    set_coordinates(({ob_co[0], ob_co[1], me_co[2]}));

	    tell_objects(LISTEN(env) - ({ me }),
			 ({ QCTNAME(me),
				" " + query_move_msg() + " a bit " +
				dslong[d] + ".\n" }),
			 MSG_SEE | MSG_WALK, me);
	    
	    return result;
	}
	else
	{
	    direct_approach(this_object(), approach_obj);
	    approach_obj=this_object();

	    return 3;
	}
    }
    
    if (d < 0 && -d >= step)
	dir = "n";
    else if (d > 0 && d >= step)
	dir = "s";
    
    if (d2 < 0 && -d2 >= step)
	dir += "e";
    else if (d2 > 0 && d2 >= step)
	dir += "w";
    
    d=member_array(dir, ds);
    if (command(dir) == -1)
    {
	if (command(ds[(d+1) % 8]) == -1)
	    if (command(ds[(d+7) % 8]) == -1)
		return -2;
    }

    return result;
}
