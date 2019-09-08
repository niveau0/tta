/* Comments:
 * - this code allows to knot an object only once to a rope.
 * - tearing is not yet implemented
 * - new holding-position is last_knot_pos+1; could cause problems
 * - only a single person can hold the rope at the same time
 */

inherit "/base/object";

#include <composite.h>
#include <event.h>
#include <living.h>
#include <macros.h>
#include <material.h>
#include <math.h>
#include <properties.h>
#include <skills.h>


/* Koennte spaeter nach /include ausgelagert werden. */
/* The indices within the knots-mapping */
#define KNOT_POS     0           /* knots position on the rope [inch] */
#define KNOT_STR     1           /* knot's strength in strength-units */
#define KNOT_TYP     2           /* knot's type */
#define PREV_KNOT    3           /* The previous knot, 0 if at beginning */
#define NEXT_KNOT    PREV_KNOT+1 /* The following knot, 0 if at end */
/* knot-types */
#define FULL_SLIDE 0 /* knot's position can slide over other knots */
#define SEMI_SLIDE 1 /* knot's position can slide between prev and next knot */
#define FIX_KNOT   2 /* knot's position cannot slide */
#define HOLDING    3 /* rope is held at this position */

#define HOLD_RATIO   3  /* strength_of_hands = full_strength/HOLD_RATIO
		         * For example: 3 means, hands have only a third of
		         * the full strength */


int rope_length, tens_strength, knot_dist,
    last_knot_pos; /* this variable serves as position where a rope is held */
mapping knots; /* contains the data for all knots */
object old_Env;

void set_knot_distance(int distance);
void set_length(int length);
void set_tens_strength(int strength);

static void
create_rope()
{
    set_name("rope");
    set_pname("ropes");
    set_short("thin rope");
    set_pshort("thin ropes");
    add_adj("thin");
    set_long("A thin, nearly ethereal, rope.\n");
    set_length(990);
    set_lwvs(0, 30, 100, 4);
    set_value(30);
    set_tens_strength(50);
    set_knot_distance(50);
    add_prop(OBJ_MATERIAL, M_SILK);
}

static void
create_object()
{
    knots = ([ ]);
    last_knot_pos = 0;
    create_rope();
    add_event(E_MOVE, "rope_move", this_object());
}

/*
 * Function name: set_length
 * Description:   set the length of the thread
 * Arguments:     length - the length in inch
 */
void set_length(int length)
{
    rope_length = length;
}

/*
 * Function name: query_length
 * Description:   returns the length of the thread
 * Returns:       the length in inches
 */
int
query_length()
{
    return rope_length;
}

/*
 * Function name: set_knot_distance
 * Description:   sets the minimum distance between two knots
 * Arguments:     distance - the distance in inches
 */
void set_knot_distance(int distance)
{
    knot_dist = distance;
}

/*
 * Function name: query_knot_distance
 * Description:   returns the minimum distance between two knots
 * Returns:       the distance in inches
 */
int query_knot_distance()
{
    return knot_dist;
}

/*
 * Function name: set_tens_strength
 * Description:   set the tensile strength
 *                it is measured in strength-units of livings
 * Arguments:     strength - the force necessary to tear the thread
 */
void
set_tens_strength(int strength)
{
    tens_strength = strength;
}

/*
 * Function name: query_tens_strength
 * Description:   query the tensile strength
 * Returns:       the tensile strength
 *                it is measured in strength-units of livings
 */
int
query_tens_strength()
{
    return tens_strength;
}

/*
 * Function name: add_knot
 * Description:   will add the object to the list of connected objects
 * Arguments:     obj - the object that gets knotted to the rope
 *                position - the position measured from the ropes
 *                           beginning [in inches]
 *                strength - the strength of the knot
 * Returns:       object that is afterwards knotted to this position or 0
 */
object
add_knot(object obj, int position, int type, int strength)
{
    int prev_pos, next_pos, i, current_pos;
    object prev_object, next_object, *ind;
    
    if (!obj || position<0 || position>rope_length)
	return 0;
    if (knots[obj])
    {
	this_player()->catch_tell("They are already knotted together.\n");
	return 0;
    }
    ind = map_indices(knots);
    if (!sizeof(ind))
    {
	knots[obj] = ({ position, strength, type, 0, 0 });
	add_event(E_MOVE_ATTEMPT,"knot_move_attempt", obj);
	return obj;
    }
    prev_pos = 0;
    next_pos = rope_length;
    prev_object = next_object = 0;
    for (i = sizeof(ind)-1; i>=0; i--)
    {
	current_pos = knots[ind[i]][KNOT_POS];
	
	if (current_pos == position)
	    return ind[i];
	if (current_pos >= prev_pos &&
	    current_pos < position )
	{
	    prev_pos = current_pos;
	    prev_object = ind[i];
	}
	else if (current_pos <= next_pos &&
		 current_pos > position)
	{
	    next_pos = current_pos;
	    next_object = ind[i];
	}
    }
    knots[obj] = ({ 0, 0, 0, 0, 0 });
    knots[obj][KNOT_POS] = position;
    knots[obj][KNOT_STR] = strength;
    knots[obj][KNOT_TYP] = type;
    knots[obj][PREV_KNOT] = prev_object;
    knots[obj][NEXT_KNOT] = next_object;
    if (next_object)
	knots[next_object][PREV_KNOT] = obj;
    if (prev_object)
	knots[prev_object][NEXT_KNOT] = obj;
    
    add_event(E_MOVE_ATTEMPT,"knot_move_attempt", obj);
    last_knot_pos = position;
    return obj;
}

/*
 * Function name: remove_knot
 * Description:   remove an existing knot from the rope
 * Arguments:     obj - an object knotted to the rope
 * Returns:       1 if success, 0 else
 */
int
remove_knot(object obj)
{
    string name;
    object tp, tmp_obj;
    
    if (!obj)
	return 0;
    tp = this_player();
    if ( !sizeof(map_indices(knots)) )
    {
	tp->catch_tell("But there is nothing knotted to the" +
	    short() + ".\n");
	return 0;
    }
    if (!knots[obj])
    {
	if (living(obj))
	    tp->catch_msg(({ QCNAME(obj), " isn't knotted to the ", 
				 short(), ".\n" }), tp, MSG_SEE);
	else
	    tp->catch_tell("It isn't knotted to the " + short() + ".\n");
	return 0;
    }

    tmp_obj = knots[obj][PREV_KNOT];
    if (tmp_obj)
	knots[tmp_obj][NEXT_KNOT] = knots[obj][NEXT_KNOT];
    
    tmp_obj = knots[obj][NEXT_KNOT];
    if (tmp_obj)
	knots[tmp_obj][PREV_KNOT] = knots[obj][PREV_KNOT];
	
    knots[obj] = 0;
    remove_event(E_MOVE_ATTEMPT,"knot_move_attempt", obj);
    return 1;
}

/*
 * Function name: long
 * Description:   adds the knotted objects to the normal description
 * Returns:       the describing string with the knotted objects
 */
string
long()
{
    int i, sz, *tp_coord;
    object tp, temp_obj, *ind;
    string out;

    /* perhaps a little skill_check? */
    out = "It is about " + MATH_INCHES_TO_STRING(rope_length) + " long.\n";
    tp = this_player();
    ind = tp->visible(map_indices(knots));
    if (sz = sizeof(ind))
    {
	tp_coord = tp->query_coordinates();
	out += "Along the " + short() + " you see:\n";
	for (i = sz-1; i>=0; i--)
	{
	    temp_obj = ind[i];
	    out += temp_obj->short() +
		   COORD_SUFFIX(tp_coord, temp_obj->query_coordinates()) + "(";
	    switch ((int)((knots[temp_obj][KNOT_POS]*5)/rope_length))
	    {
	      case (0):
		  out += "beginning)";
		  break;
	      case (1):
		  out += "second fifth)";
		  break;
	      case (2):
		  out += "middle)";
		  break;
	      case (3):
		  out += "third fifth)";
		  break;
	      default :
		  out += "end)";
	    }
	    if (i == 1)
		out += " and ";
	    else
		if (i != 0)
		    out += ", ";
	}
    }
    return (::long() + out);
}

/*
 * Function name: query_knots
 * Description:   returns a mapping with the knots, their neighbours
 *                and their position and other data
 * Returns:       mapping with ([ knotted object:
 *                                ({ position, previous knot, next knot })
 *                               ...
 *                              ])
 */
mapping
query_knots()
{
    return knots;
}

/*
 * Function name: bind_cmd
 * Description:   handles all effects upon binding a living with this rope
 * Arguments:     obj - the binding object
 * Returns:       1 if success, -1 else
 */
int
bind_cmd(object obj)
{
    
    obj->catch_tell("Not yet implemented for ropes ... you failed.\n");
    return -1;
}

/*
 * Function name: pull_cmd
 * Description:   handles all effects of pulling the rope towards the
 *                pulling object (one side after the other)
 * Arguments:     obj - the pulling object
 * Returns:       1 if success, -1 else
 */
int
pull_cmd(object obj)
{
    obj->catch_tell("Not yet implemented for ropes ... you failed.\n");
    return -1;
}

/*
 * Function name: knot_cmd
 * Description:   scans and handles the string after the knot command
 * Arguments:     obj - the object to knot to the rope
 *                pos - a string where to knot it to the rope
 *                tp - the knotting person
 * Returns:       1 if success, -1 else
 */
int
knot_cmd(object obj, string pos, object tp)
{
    int position, knot_str;
    object act_knot, *ind;

    if (!obj || !tp)
	return -1;
    switch (pos)
    {
      case "beginning":
	  position = 0;
	  break;	  
      case "middle":
	  position = (int)rope_length/3;
	  break;
      case "end":
	  position = rope_length - (int)(rope_length/3);
	  break;
      default:
	  /* the beginning of the rope is the default */
	  position = 0;
	  if (pos)
	      tp->catch_tell("You cannot find the " + pos + " of the " +
			     short() +
			     ", so you try knotting to it's beginning.\n");
    }
    ind = map_indices(knots);
    if (sizeof(ind))
    {
	/* find a suitable free spot starting from position, that means
	 * a spot whith free knot_dist in both directions */
	act_knot = ind[0];
	/* find the knot closest to the beginning */
	while (knots[act_knot][PREV_KNOT])
	    act_knot = knots[act_knot][PREV_KNOT];
	while (act_knot)
	{
	    if (knots[act_knot][KNOT_POS] + knot_dist > position)
		if ( (position = knots[act_knot][KNOT_POS]+knot_dist) >
		    rope_length)
		{
		    /* no free spot found */
		    tp->catch_tell("You cannot find a free spot "+
				   "for knotting up to the end.\n");
		    return -1;
		}
	    
	    /* checking the distance to the following knot (if existing) */
	    if (act_knot = knots[act_knot][NEXT_KNOT])
	    {
		if (knots[act_knot][KNOT_POS]-knot_dist > position)
		    /* fitting position found */
		    act_knot = 0;
	    }
	}
    }
    if ((knot_str = 40 + tp->query_skill_bonus(SK_ROPE_MASTERY) +
	 roll_dice(-1)) < 100)
    {
	tp->catch_tell("Your thoughts wander off and you fumble the knot.\n");
	return -1;
    }
    else if ( obj != add_knot(obj, position, FIX_KNOT,
			      (tp->query_skill_rank(SK_ROPE_MASTERY) +
			       knot_str/100)) )
    {
	tp->catch_tell("You failed to knot the "+obj->short()+
		       " to the "+short()+".\n");
	return -1;
    }
    /* holding the rope it the new knotting position */
    if (knots[old_Env])
	knots[old_Env][KNOT_POS] = last_knot_pos;
    tp->skill_usage(SK_ROPE_MASTERY);
    return 1;
}

/*
 * Function name: untie_cmd
 * Description:   scans and handles the string after the untie command
 * Arguments:     msg - the string about what to untie
 *                tp - the untieing person
 * Returns:       1 if success, -1 else
 */
int
untie_cmd(object obj, object tp)
{
    int temp_str;

    if ((temp_str = tp->query_skill_bonus(SK_ROPE_MASTERY) +
	 50 + roll_dice(-1) ) > 100)
    {
	knots[obj][KNOT_STR] -= tp->query_skill_rank(SK_ROPE_MASTERY) +
				temp_str/100;
	tp->skill_usage(SK_ROPE_MASTERY);
	if (knots[obj][KNOT_STR] < 1)
	{
	    if (!remove_knot(obj))
	    {
		tp->catch_tell("It is impossible to remove that knot.\n");
		return -1;
	    }
	    /* knot has been successfully removed */
	    return 1;
	}
	else
	{
	    tp->catch_tell("You only managed to unfasten the knot a bit.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " tries to untie a knot.\n" }),
			 MSG_SEE, tp);
	}
    }
    else if (temp_str < -99)
    {
	knots[obj][KNOT_STR] -= temp_str/100;
	tp->catch_tell("You thoughts wander off and you make the knot"+
		       "even tighter.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " fumbles at a knot.\n" }),
		     MSG_SEE, tp);
    }
    return -1;
}

/*
 * Function name: reset_chain
 * Description:   sets all objects within the chain to their coords
 * Arguments:     chain - a list of objects and coords
 *                according to make_chain
 */
private void
reset_chain(mixed chain)
{
    int szmc;
    
    for (szmc = sizeof(chain)-1; szmc >= 0; szmc--)
    {
	chain[szmc][0]->set_coordinates(chain[szmc][1]);
    }
}

/*
 * Function name: make_chain
 * Description:   builds a list of knots that will be moved upon a pulling
 *                force towards a coordinate, 
 * Arguments:     obj - object that is pulled first (new coord have to be set)
 *                force - the force to pull with
 *                direction - NEXT_KNOT or PREV_KNOT
 * Returns:       list of ({ stopping reason , ({ move_chain }) })
 *                stopping reason:
 *                   >= 0 no problems (remaining force is returned)
 *                   -1 out of force before everything moved
 *                   -2 E_MOVE_ATTEMPT blocked move
 *                   -3 obj was pulled to invalid coordinates
 */
private mixed
make_chain(object obj, int force, int direction)
{
    float factor, new_dist;
    int i, result, rope_dist, *pull_coord, *knot_coord;
    mixed move_chain, old_coord;
    object act_knot, new_Env, old_knot;

    move_chain = ({ });
    old_knot = obj;
    act_knot = knots[old_knot][direction];
    result = 0;
    while (!result && act_knot)
    {
	knot_coord = act_knot->query_coordinates();
	pull_coord = old_knot->query_coordinates();
	rope_dist = ABS(knots[act_knot][KNOT_POS]-knots[old_knot][KNOT_POS]);

	if (!distance(pull_coord, knot_coord, rope_dist))
	{
	    /* following knot needs to be moved */
	    old_coord = ({ knot_coord[0],knot_coord[1],knot_coord[2] });
	    /* computing new coords for MOVE_ATTEMPT */
	    new_dist = SQRT((knot_coord[0]-pull_coord[0])*
			    (knot_coord[0]-pull_coord[0]) +
			    (knot_coord[1]-pull_coord[1])*
			    (knot_coord[1]-pull_coord[1]) +
			    (knot_coord[2]-pull_coord[2])*
			    (knot_coord[2]-pull_coord[2])  );
	    factor = ((float)rope_dist)/new_dist;
	    for (i = 0; i < 3; i++)
	    {
		knot_coord[i] = pull_coord[i]+
				(int)(factor*
				      (float)(knot_coord[i]-pull_coord[i]));
	    }
	    /* check if the new coord have a valid environment */
	    if ( !(new_Env = E(act_knot)->query_leaving_room(knot_coord)) )
		result = -3;
	    else
	    {
		/* a switch(knot-type) should begin here */
		
		/* usage of force needed to pull gets determined */
		if (living(act_knot))
		    /* livings always(?) struggle against pull */
		    force -= act_knot->query_stat_bonus(STAT_ST);
		/* force needed for moving the mass */
		force -= (int)act_knot->query_weight()/1000;
		if (force < 0)
		    /* not enough force */
		    result = -1;
		else
		{
		    /* check, if another obj blocks move via E_MOVE_ATTEMPT */
		    remove_event(E_MOVE_ATTEMPT,"knot_move_attempt", act_knot);
		    if (act_knot->call_event(E_MOVE_ATTEMPT,
					     act_knot, E(act_knot), new_Env))
			result = -2;
		    add_event(E_MOVE_ATTEMPT,"knot_move_attempt", act_knot);
		}
	    }
	    move_chain += ({ ({ act_knot, old_coord, new_Env }) });
	    old_knot = act_knot;
	    act_knot = knots[old_knot][direction];
	}
	else
	    result = force;
    }
    
    if (result < 0)
    {
	reset_chain(move_chain);
	move_chain = 0;
    }
    else
	result = force;
    return ({ result, move_chain });
}

/*
 * Function name: react_blocked_move
 * Description:   delivers output and action according to the reason
 * Arguments:     obj - the pulling object
 *                reason - for the values see make_chain()
 */
private void
react_blocked_move(object obj, int reason)
{
    /* Not yet implemented ...
     *                   -1 out of force before everything moved
     *                   -2 E_MOVE_ATTEMPT blocked move
     *                   -3 obj was pulled to invalid coordinates
     */

    obj->catch_tell("The " + short() +
		    " gets tense and prevents your movement.\n");
}

/*
 * Function name: knot_move_attempt
 * Description:   handle the fact, that a knotted object tried to move
 *                moving other knotted obj's and so on
 * Arguments:     obj - the pulling object
 *                from - the former environment
 *                to - the new environment
 * Returns:       integer != 0 if move was blocked, else 0
 */
int
knot_move_attempt(object obj, object from, object to)
{
    int pull_force, szmc;
    mixed move_chain, result, temp_return;
    object act_knot;

    /* determine the pulling force of the obj */
    pull_force = obj->query_stat_bonus(STAT_ST);
    
    temp_return = make_chain(obj, pull_force, PREV_KNOT);
    if ( (result = temp_return[0]) > -1)
    {
	/* move was okay */
	move_chain = temp_return[1];
	temp_return = make_chain(obj, result, NEXT_KNOT);
	if ( (result = temp_return[0]) > -1)
	{
	    /* move was okay */
	    move_chain += temp_return[1];
	    /* move all elements of the move_chain */
	    for (szmc = sizeof(move_chain)-1; szmc >= 0; szmc--)
	    {
		act_knot = move_chain[szmc][0];
		if (act_knot->query_prop(IS_HEAP))
		    /* move_heap does the full moving */
		    act_knot->move_heap(move_chain[szmc][2]);
		else
		    act_knot->move_now(move_chain[szmc][2], E(act_knot));
		if (living(act_knot))
		{
		    obj->catch_msg( ({ "You pull ", QNAME(act_knot),
					  " towards you.\n" }), obj, MSG_SEE);
		    act_knot->catch_msg(
			({ QCTNAME(obj), " pulls you along with the ",
			       short(), ".\n" }), obj, MSG_SEE);
		}
		else
		    obj->catch_tell("You pull the "+act_knot->short()+".\n");
		tell_objects(LISTEN(E(act_knot)) - ({ obj, act_knot }),
			     ({ QCTNAME(obj), " pulls ", QNAME(act_knot),
				    " with a ", short(), ".\n" }),
			     MSG_SEE, obj);
	    }
	    return 0;
	}
	/* reset the successful chain of the first direction */
	reset_chain(move_chain);
    }
    /* move was impossible => branch on different reasons */
    react_blocked_move(obj, result);
    return 1;
}

/*
 * Function name: rope_move
 * Description:   handles effencts upon moving the rope
 *                if the rope is taken, the living's move-attempt has to be
 *                observed
 */
void
rope_move(object obj, object from, object to)
{
    if (to == old_Env)
	return;
    if (knots[old_Env])
	if (knots[old_Env][KNOT_TYP] = HOLDING)
	    remove_knot(old_Env);
    old_Env = to;
    /* the rope is always held at the position of the last knot */
    if (living(to) && !knots[to])
	add_knot(to, ++last_knot_pos, HOLDING,
		 (int)(to->query_stat_bonus(STAT_ST)/HOLD_RATIO));
}
