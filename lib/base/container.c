inherit "object";  
inherit "/system/inventory";

#include <analyse.h>
#include <composite.h>
#include <event.h>
#include <language.h>
#include <liquid.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>

private int     cont_cur_light,  /* current light status in cont. */
                cont_liquid,     /* current liquid amount in cont. */
                cont_cur_weight, /* current weight status in cont. */
	        cont_cur_volume, /* current volume status in cont. */
                cont_max_weight, /* maximum weight status in cont. */
                cont_max_volume, /* maximum volume status in cont. */
                type;            /* the liquid type (see liquid.h) */

private string  ltype,         /* liquid type */
                llook;         /* look of the liquid */
private object  cont_linkroom; /* allows a room to be the inventory
				  of this container */
private mapping status;        /* remember analysed liquid in container
				  for all players */
/*
 * Function name: create_container
 * Description:   redefinable create function 
 */
static void
create_container()
{
}

/*
 * Function name: create_object
 * Description:   Create the container (constructor)
 */
static nomask void
create_object()
{
    add_prop(IS_CONT, 1);	/* This is a container */
    status = ([]);
    create_container();
}

/*
 * Function name: show_inside
 * Description:   gives description of contents
 * Arguments:     tp - the looking player
 * Returns:       string
 */
nomask string
show_inside(object tp)
{
    string str;
    object *attached, *ob_inside;
    
    if (living(this_object()) || 
	(!query_prop(CONT_TRANSP) && query_prop(CONT_CLOSED)))
	return 0;

    if (objectp(cont_linkroom)) /* check linked room to containers */
	ob_inside = I(cont_linkroom, 0);
    else
	ob_inside = I(this_object(), 0);
    if (sizeof(ob_inside))
    {
	/* don't show attached objects as inside objects */
	if (attached = this_object()->attached())
	    ob_inside -= attached;
	ob_inside = tp->visible(ob_inside);
    }
    str = "";
    if (ltype)
    {
	str = "You see " + (status[tp] == ITEM_ANALYSED ? ltype :
			    "some " + llook + " liquid") + " inside. ";
	switch (cont_liquid * 100 / (cont_max_volume - ::query_volume()))
	{
	  case 0..5:
	      str += "It's almost empty.\n";
	      break;
	  case 6..30:
	      str += "There's one quarter left.\n";
	      break;
	  case 31..70:
	      str += "It's half filled.\n";
	      break;
	  case 71..95:
	      str += "There are three quarters left.\n";
	      break;
	  default:
	      str += "It's full.\n";
	      break;
	}
	if (!sizeof(ob_inside))
	    return str;
	return str + "It also contains:\n" +
	    BS(COMPOSITE(ob_inside, tp), 75, 1) + ".\n";
    }
    if (!sizeof(ob_inside))
	return "The " + short() + " is empty.\n";
    return "It contains:\n" +
	BS(COMPOSITE(ob_inside, tp), 75, 1) + ".\n";
}

/* Function name: query_analysed
 * Description:   shows the success of a player who tried to analyse the liquid
 * Returns:       success of the player (see analyse.h)
 * Arguments:     tp - the analysing player
 */
nomask int
query_analysed(object tp)
{
    return status[tp];
}

/*
 * Function name: set_liquid_type
 * Description:   set the liquid inside the object
 * Arguments:     t - the liquid (see <liquid.h>)
 *                look - how the liquid looks
 *                l - the name of the liquid
 */
nomask void
set_liquid_type(int t, string look, string l)
{
    status = ([]);
    llook = look;
    type = t;
    ltype = l;
}

/*
 * Function name: query_liquid_int
 * Description:   gives the liquid
 * Returns:       int with the number of the liquid defined in liquid.h
 */
nomask int
query_liquid_int()
{
    return type;
}

/*
 * Function name: query_liquid_look
 * Description:   gives the liquid's look
 * Returns:       string with the look of the liquid
 */
nomask string
query_liquid_look()
{
    return llook;
}

/*
 * Function name: query_liquid_type
 * Description:   gives the liquid 
 * Returns:       string with the name of the liquid
 */
nomask string
query_liquid_type()
{
    return ltype;
}

/*
 * Function name: query_light
 * Description:   Returns the light status in this container
 * Returns:	  Light value
 */
nomask int
query_light()
{
    if (query_prop(CONT_TRANSP))
    	return cont_cur_light + ::query_light();
    return ::query_light();
}

/*
 * Function name: query_max_weight
 * Description:   Returns the maximum weight the container can hold.
 * Returns:	  Weight value
 */
int
query_max_weight()
{
    return cont_max_weight;
}

/*
 * Function name: set_max_volume
 * Description:   Returns the maximum volume the container can hold.
 * Returns:	  Volume value
 */
int
query_max_volume()
{
    return cont_max_volume;
}

/*
 * Function name: query_weight
 * Description:   Returns the accumulated weight of the container.
 * Returns:	  Weight value
 */
nomask int
query_weight()
{
    return cont_cur_weight + cont_liquid + ::query_weight();
}

/*
 * Function name: query_volume
 * Description:   Returns the volume of the container. 
 * Returns:	  Volume value
 */
nomask int
query_volume()
{
    if (query_prop(CONT_RIGID)) 
	return query_max_volume();
    return cont_cur_volume + cont_liquid + ::query_volume();
}

/*
 * Function name: query_cont_weight
 * Description:   Returns the weight of the container itself.
 * Returns:	  Weight value
 */
nomask int
query_cont_weight()
{
    return ::query_weight();
}

/*
 * Function name: query_cont_volume
 * Description:   Returns the volume of the container itself. 
 * Returns:	  Volume value
 */
nomask int
query_cont_volume()
{
    if (query_prop(CONT_RIGID)) 
	return query_max_volume();
    return ::query_volume();
}

/*
 * Function name: query_internal_light
 * Description:   Returns the lightvalue of object internal to this container
 * Returns:	  Lightvalue
 */
nomask int
query_internal_light()
{
    return cont_cur_light;
}

/*
 * Function name: query_internal_liquid
 * Description:   Returns how much liquid is inside the container
 * Returns:	  integer
 */
nomask int
query_internal_liquid()
{
    return cont_liquid;
}

/*
 * Function name: query_internal_weight
 * Description:   Returns the weight of the items in the container.
 * Returns:	  Weight value
 */
nomask int
query_internal_weight()
{
    return cont_cur_weight + cont_liquid;
}

/*
 * Function name: query_internal_volume
 * Description:   Returns the volume of the items in the container. 
 * Returns:	  Volume value
 */
nomask int
query_internal_volume()
{
    return cont_cur_volume + cont_liquid;
}

/*
 * Function name: set_max_weight
 * Description:   Set the maximum weight the container can hold.
 * Arguments:     Weight value
 */
nomask void
set_max_weight(int val)
{
    cont_max_weight = val;
}

/*
 * Function name: set_max_volume
 * Description:   Set the maximum volume the container can hold. 
 * Arguments:     Volume value
 */
nomask void
set_max_volume(int val)
{
    cont_max_volume = val;
}

/*
 * Function name: weight_left
 * Description:   Returns the weight left to fill.
 * Returns:	  non_negative integer (the left weight)
 */
nomask int
weight_left()
{
    return query_max_weight() - query_weight();
}

/*
 * Function name: volume_left
 * Description:   Returns the volume left to fill.
 * Returns:	  non_negative integer (the left volume)
 */
nomask int
volume_left()
{
    return query_max_volume() -
	(cont_cur_volume + cont_liquid + ::query_volume());
}

/*
 * Function name: set_room
 * Description:   Connects a room to the internals of the container.
 * Arguments:	  room: The room object
 */
nomask void
set_room(object room)
{
    cont_linkroom = room;
}

nomask object
query_room()
{
    return cont_linkroom;
}

/* 
 * Function name: update_internal
 * Description:   Updates the light, weight and volume of things inside
 *                also updates a possible environment.
 * Arguments:     l: Light diff.
 *		  w: Weight diff.
 *		  v: Volume diff.
 */
nomask void
update_internal(int l, int w, int v)
{
    int    p;
  
    p = query_prop(CONT_LESS_WEIGHT); /* allows container to lower weight */
    
    if (p < 101 && p > 9)
	w = (w*p)/100;

    cont_cur_light  += l;
    cont_cur_weight += w;
    cont_cur_volume += v;

    if (!environment())
	return;

    if (!query_prop(CONT_TRANSP))
	l = 0;
    
    if (query_prop(CONT_RIGID))
	v = 0;

    if (l || w || v)
	environment()->update_internal(l, w, v);
}

/*
 * Function name: notify_change_prop
 * Description:   This function is called when a property in an object
 * 		  in the inventory has been changed.
 * Arguments:	  prop - The property that has been changed.
 *		  val  - The new value.
 *		  old  - The old value.
 */
nomask void
notify_change_prop(mixed prop, mixed val, mixed old)
{
    int n;

    if (prop != CONT_TRANSP)
	return;
    if (old != val && environment())
    {
	n = previous_object()->query_internal_light();
	if (!val)
	    n = -n;
	environment()->update_internal(n, 0, 0);
    }
}

/*
 * Function name: set_fill
 * Description:   set how much liquid is inside
 * Arguments:     f - the amount
 */
static nomask void
set_fill(int f)
{
    if (f > volume_left() || f > weight_left())
	error("Too much liquid");
    
    cont_liquid = f;
    if (cont_liquid <= 0)
    {
	status = ([]);
	ltype = 0;
	llook = 0;
    }
}

/*
 * Function name: add_fill
 * Description:   add/sub amount, liquid type must be set before
 * Arguments:     f - how much to fill/empty
 * Returns:       how much can be filled/removed
 */
int
add_fill(int f)
{
    if (!ltype)
	return 0;

    if (-f > cont_liquid)
    {
	f = -cont_liquid;
	cont_liquid = 0;
	ltype = 0;
	llook = 0;
	status = ([]);
    }
    else
    {
	int l;

	l = volume_left();
	if (f > l)
	    f = l;

	l = weight_left();
	if (f > l)
	    f = l;
	cont_liquid += f;
    }
    if (cont_liquid <= 0)
    {
	ltype = 0;
	llook = 0;
	status = ([]);
    }
    return f;
}

/*
 * Function name: fill_cmd
 * Description:   called if filling was tried
 * Arguments:     ob - from which object this one gets the liquid
 * Returns:       1 if success, -1 else
 */
int
fill_cmd(object ob)
{
    int i;
    string l, ob_look;
    object tp, *looking;
    mapping father_status;
    
    if (!ob)
	return 0;
    tp = this_player();
    if (!(l = ob->query_liquid_type()))
    {
	tp->catch_tell("The " + ob->short() +
		       " does not contain any liquid.\n");
	return -1;
    }
    if (ltype && (!(ob->query_analysed(tp) == ITEM_ANALYSED) ||
		  !(query_analysed(tp) == ITEM_ANALYSED)))
    {
	tp->catch_tell("You should have analysed both liquids when you want " +
		       "to mix them.\n");
	return -1;
    }
    if (ltype && ltype != l)
    {
	tp->catch_tell("You shouldn't mix two different liquids.\n");
	return -1;
    }
    ob_look = ob->query_liquid_look();
    if (!ltype)
	set_liquid_type(ob->query_liquid_int(), ob_look, l);
    if (volume_left() > 0)
    {
	looking = LISTEN(E(tp));
	father_status = ob->query_status();
	looking = map_indices(father_status) & looking;
	for (i=sizeof(looking);i--;)
	    if (VISIBLE(looking[i], tp))
		status[looking[i]] = father_status[looking[i]];
    }
    if (!add_fill(-ob->add_fill(-volume_left())))
    {
	tp->catch_tell("But the " + short() + " is full.\n");
	return -1;
    }
    tp->catch_tell("You fill the " + (status[tp] == ITEM_ANALYSED ? l :
				      ob_look + " liquid") +
		   " into the " + short() + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " fills some " + ob_look + " liquid into " +
			LANG_ADDART(short()) + ".\n" }), MSG_SEE, tp);
    return 1;
}

/*
 * Function name: drink_cmd
 * Description:   called if drinking was tried
 * Returns:       1 if success, -1 else
 */
nomask int
drink_cmd()
{
    int    l;
    object tp;

    tp = this_player();

    if (!query_liquid_type())
    {
	tp->catch_tell("It's empty.\n");
	return -1;
    }
    l = tp->add_water(cont_liquid);
    if (l < 0)
    {
	tp->catch_tell("You cannot drink more.\n");
	return -1;
    }
    add_fill(-l);
    tp->echo("You take a good swallow from the " + short() + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " drinks from ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    
    LIQUID_EFFECT(query_liquid_int(), l, tp);
    return 1;
}

/*
 * Function name: pour_out_cmd
 * Description:   called if the container was tried to empty
 * Returns:       1/-1 on success
 */
nomask int
pour_out_cmd(object tp)
{
    if (!ltype)
    {
	tp->catch_tell("But the " + short() + " is already empty.\n");
	return -1;
    }
    if (query_prop(OBJ_NO_GET))
    {
	tp->catch_tell("You can't pour out the " + short() + ".\n");
	return -1;
    }
    set_fill(0);
    tp->echo("You pour the " + short() + " out.\n");    
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " pours ", QTNAME(this_object()),
			" out.\n" }), MSG_SEE, tp);
    return 1;
}

/* Function name: analyse_cmd
 * Description:   called if container and its liquid is tried to analyse
 * Returns:       1/-1 on success
 * Arguments:     tp - the analysing player
 */
nomask int
analyse_cmd(object tp)
{
    int result, liq;

    if (!ltype)
	return -1;
    
    if (!tp)
	return -1;

    if (status[tp] == ITEM_TRIED)
    {
	tp->catch_tell("You already tried to analyse this kind.\n");
	return -1;
    }
    if (status[tp] == ITEM_ANALYSED)
    {
	tp->catch_tell("You find out: The liquid is " + ltype + ".\n");
	return 1;
    }
    if ((liq = query_liquid_int()) >= LIQ_ALCOHOL_FIRST &&
	liq <= LIQ_ALCOHOL_LAST)
	result = tp->skill_check(SK_COOKERY, 30, 0);
    else if (liq >= LIQ_POTION_FIRST && liq <= LIQ_POTION_LAST)
	result = tp->skill_check(SK_COOKERY, -20, 0) +
		 tp->skill_check(SK_HERB_LORE, 0, 0);
    else if (liq >= LIQ_OTHER_FIRST && liq <= LIQ_OTHER_LAST)
	result = tp->skill_check(SK_COOKERY, 0, 0);
    else if (liq >= LIQ_POISON_FIRST && liq <= LIQ_POISON_LAST)
	result = tp->skill_check(SK_COOKERY, -20, 0) +
		 tp->skill_check(SK_POISON_LORE, 0, 0);
    
    if (result > 0)
    {
	status[tp] = ITEM_ANALYSED;
	tp->catch_tell("You find out: The liquid is " + ltype + ".\n");
	return 1;
    }
    status[tp] = ITEM_TRIED;
    tp->catch_tell("You fail to analyse the liquid.\n");
    return -1; 
}

/* Function name: set_status
 * Description:   sets the mapping of players that tried to analyse the liquid
 * Arguments:     st - the mapping of players and their success (see analyse.h)
 */
nomask void
set_status(mapping st)
{
    status = st;
}

/* Function name: query_status
 * Description:   returns the mapping of players who tried to analyse the
 *                liquid and their success
 * Returns:       the status mapping
 */
nomask mapping
query_status()
{
    return status;
}

