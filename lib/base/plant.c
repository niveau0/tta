inherit "food";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

private int     phase,           /* the actual grow-phase of this plant */
                max_phase,       /* the maximum grow-phase for this plant */
                burn;            /* burning status */

private mapping plant_data;      

static nomask void change_plant(int new_phase);
nomask void set_plant_data(mapping m);

/*
 * Function name: create_plant
 * Description:   redefinable plant-constructor function
 */
static void
create_plant()
{
}

static nomask void
create_food()
{
    set_name("plant");
    
    set_plant_data(
	([
	    PLANT_LONG : ([
		1 : "An ordinary plant.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some ordinary plants.\n",
		]),
	    PLANT_SHORT : ([
		1 : "plant",
		]),
	    PLANT_PSHORT : ([
		1 : "plants",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		]),
	    PLANT_VALUE : ([
		1 : 0,
		]),
	    PLANT_FOOD : ([
		1 : 10,
		]),
	    ]));
 
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OAK);
    max_phase = 1;
    add_prop(IS_PLANT, 1);
    create_plant();
    set_heap_size(1);
    
    change_plant(1); /* set start values */
    phase = 1;
    add_event(E_HEAP_MERGE, "plant_merge", this_object());
}

/*
 * Function name: query_heap_id
 * Description:   gives the heap id
 * Returns:       the id
 */
string
query_heap_id()
{
    if (burn > 0)
	return ::query_heap_id() +  "_" + phase + "_" + burn;
    return ::query_heap_id() + "_" + phase;
}

/*
 * Function name: long
 * Description:   Describe the object
 * Returns:	  string
 */
string
long()
{
    if (burn > 0)
	return ::long() + (query_heap_size() > 1
			   ? "They are" : "It is") + " burning.\n";
    return ::long();
}

/*
 * Function name: short
 * Description:   Return the short description of the object.
 * Returns:       string
 */
string
short()
{
    if (burn > 0)
	return "burning " + ::short();
    return ::short();
}

/*
 * Function name: plural_short
 * Description:   Return the short description of the object.
 * Returns:       string
 */
string
plural_short()
{
    if (burn > 0)
	return "burning " + ::plural_short();
    return ::plural_short();
}

/*
 * Function name: set_max_phase
 * Description:   sets maximum number of grow/shrink phases
 * Arguments:     m - maximum grow phases
 */
static nomask void
set_max_phase(int m)
{
    max_phase = m;
}

/*
 * Function name: query_grow_phase
 * Description:   gives actual grow phase
 * Returns:       integer
 */
nomask int
query_grow_phase()
{
    return phase;
}

/*
 * Function name: set_plant_data
 * Description:   sets the grow data for the plant
 * Arguments:     m - mapping with all grow data
 */
nomask void
set_plant_data(mapping m)
{
    if (IS_CLONE)
	MASTER->set_plant_data(m);
    else
	plant_data = m;
}

/*
 * Function name: query_plant_data
 * Description:   get the grow data for the plant
 * Returns:       mapping with all grow data
 */
nomask mapping
query_plant_data()
{
    if (IS_CLONE)
	return MASTER->query_plant_data();
    return plant_data;
}

/*
 * Function name: change_plant
 * Description:   sets new values for a new grow-phase
 * Arguments;     new_phase - new grow phase (old is not changed for now)
 */
static nomask void
change_plant(int new_phase)
{
    int     i, v, *wvs;
    mapping data, pdata;
    mixed   old, new;

    pdata = MASTER->query_plant_data();
    
    if (mappingp(data = pdata[PLANT_NAME]))
    {
	if (new = data[new_phase])
	{
	    if (old = data[phase])
	    {
		if (pointerp(old))
		    for (i=sizeof(old); i--;)
			remove_name(old[i]);
		else
		    remove_name(old);
	    }
	    if (pointerp(new))
		for (i=sizeof(new); i--;)
		    add_name(new[i]);
	    else
		add_name(new);
	}
    }
    if (mappingp(data = pdata[PLANT_PNAME]))
    {
	if (new = data[new_phase])
	{
	    if (old = data[phase])
	    {
		if (pointerp(old))
		    for (i=sizeof(old); i--;)
			remove_pname(old[i]);
		else
		    remove_pname(old);
	    }
	    if (pointerp(new))
		for (i=sizeof(new); i--;)
		    add_pname(new[i]);
	    else
		add_pname(new);
	}
    }
    if (mappingp(data = pdata[PLANT_ADJ]))
    {
	if (new = data[new_phase])
	{
	    if (old = data[phase])
	    {
		if (pointerp(old))
		    for (i=sizeof(old); i--;)
			remove_adj(old[i]);
		else
		    remove_adj(old);
	    }
	    if (pointerp(new))
		for (i=sizeof(new); i--;)
		    add_adj(new[i]);
	    else
		add_adj(new);
	}
    }
    if (mappingp(data = pdata[PLANT_LONG]) &&
	(new = data[new_phase]))
	set_long(new);
    if (mappingp(data = pdata[PLANT_PLONG]) &&
	(new = data[new_phase]))
	set_plong(new);
    if (mappingp(data = pdata[PLANT_SHORT]) &&
	(new = data[new_phase]))
	set_short(new);
    if (mappingp(data = pdata[PLANT_PSHORT]) &&
	(new = data[new_phase]))
	set_pshort(new);
    if (mappingp(data = pdata[PLANT_WVS]) &&
	(wvs = data[new_phase]))
	set_lwvs(query_light(), wvs[0], wvs[1], wvs[2]);
    if (mappingp(data = pdata[PLANT_VALUE]) &&
	(v = data[new_phase]))
	set_value(v);
}

/*
 * Function name: do_grow
 * Description:   let the plant grow
 * Arguments:     amount - how many plants in heap should grow
 * Returns:       0 - the plant cannot grow further
 *                1 - Ok, plant grows
 */ 
nomask int
do_grow(int amount)
{
    if (amount < 1 || phase >= max_phase)
	return 0;
    if (query_heap_size() > amount)
    {
	/* split all plants not growing */
	split_heap(query_heap_size() - amount);
	if (living(E(this_object())))
	    environment()->add_tool(this_object(), environment()->
				    query_hold_slot(this_object()));
	else
	    move(E(this_object())); 
    }
    change_plant(phase + 1);
    phase++;
    return 1;
}

/*
 * Function name: do_shrink
 * Description:   let the plant shrink
 * Arguments:     amount - how many plants in heap should grow
 * Returns:       0 - the plant cannot shrink further
 *                1 - Ok, plant grows
 */ 
nomask int
do_shrink(int amount)
{
    if (amount < 1 || phase <= 1)
	return 0;
    if (query_heap_size() > amount)
    {
	/* split all plants not shrinking */
	split_heap(query_heap_size() - amount);
	if (living(E(this_object())))
	    environment()->add_tool(this_object(), environment()->
				    query_hold_slot(this_object()));
	else
	    move(E(this_object())); 
    }
    change_plant(phase - 1);
    phase--;
    return 1;
}

/*
 * Function name: do_seed
 * Description:   let the plant seed itself
 */	
void
do_seed()
{
    int    *co, step;
    object seed, room;

    if (!(co = query_coordinates()))
	return;
    
    step = (random(60) * random(6));
    switch (random(4))
    {
      case 1:
	  co[0] = co[0] + step;
	  break;
      case 2:
	  co[0] = co[0] - step;
	  break;
      case 3:
	  co[1] = co[1] + step;
	  break;
      case 4:
	  co[1] = co[1] - step;
	  break;
    }
    seed = clone_object(MASTER);
    if (room = E(this_object())->query_leaving_room(co))
    {
	seed->set_coordinates(co);
	seed->move(room);
    }
    else
    {
	seed->set_coordinates(query_coordinates());
	seed->move(E(this_object()));
    }
}

/*
 * Function name: query_burning
 * Description:   is this object burning.
 * Returns:       integer - burning status
 */
nomask int
query_burning()
{
    return burn;
}

nomask void
plant_merge(object orig, int num)
{
    if (orig && (burn = orig->query_burning()) > 0)
    {
	call_out("fade", query_weight() - burn);
	add_lwvs(1, 0, 0, 0);
    }
}

/*
 * Function name: light_cmd
 * Description:   burn the plant
 */	
nomask int
light_cmd()
{
    object tp;
    
    tp = this_player();
    
    if (burn > 0)
    {
	tp->catch_tell("The " + short() + " is already burning!\n");
	return -1;
    }
    
    if (!IN_ARRAY(query_prop(OBJ_MATERIAL), M_INFLAMMABLE))
	return 0;

    if (random(100) > 50)
    {
	burn = 1;
	if (living(E(this_object())))
	    /* split from burning */
	    environment()->add_tool(this_object(), environment()->
				    query_hold_slot(this_object()));
	else
	    move(E(this_object())); 
	burn = 0;
	
	tp->catch_tell("You set the " + short() + " on fire.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " sets ", QNAME(this_object()),
			    " on fire.\n" }), MSG_SEE);
	return 1;
    } 

    tp->catch_tell("You tried to set the " + short() +
		   " on fire, but nothing happens.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " tried to set ", QNAME(this_object()),
		      " on fire, but failed.\n" }), MSG_SEE);
    return -1;
}

/*
 * Function name: extinguish
 * Description:   put the plant out.
 */
nomask int
extinguish_cmd()
{
    int    fade;
    object tp;

    tp = this_player();
    
    if (burn < 1)
	return 0;
    if (burn > query_weight() / 5)
    {
	tp->catch_tell("The fire has become to big to be extinguished!\n");
	return -1;
    }
    fade = remove_call_out("fade"); /* save burn time */
    burn = 0;
    if (living(E(this_object())))
	/* split from burning */
	environment()->add_tool(this_object(), environment()->
				query_hold_slot(this_object()));
    else
	move(E(this_object()));
    burn = query_weight() - fade;
    call_out("fade", fade);
    
    tp->catch_tell("You extinguish the " + short() + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " extinguishes ",
		      QTNAME(this_object()),".\n" }),MSG_SEE);
    return 1;
}

/*
 * Function name: fade
 * Description:   called when fire burned the plant
 */
static nomask void
fade()
{
    object env;
    
    if (burn < 1)
	return;
    env = E(this_object());
    if (living(env))
    {
	tell_objects(LISTEN(E(env)) - ({ env }),
		     ({ QCTNAME(this_object()), " of ", QTNAME(env),
			    " burned to ashes.\n" }), MSG_SEE, 0);
	env->catch_msg("Your " + short() + " burned to ashes.\n",
		       0, MSG_SEE);
    }
    else
    {
	tell_objects(LISTEN(env),
		     ({ QCTNAME(this_object()), " burned to ashes.\n" }),
		     MSG_SEE, 0);
    }
    remove_object();
}
