inherit "object/burn";
inherit "object/props";
inherit "object/spells";
inherit "misc/pull";

#include <config.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <move.h>
#include <properties.h>
#include <slots.h>

#define IS_NAME       1
#define IS_PNAME      2
#define IS_ADJ        4
#define IS_INGREDIENT 8

void set_pname(string pname);
void add_pname(string pname);

int		       obj_weight,
	               obj_volume,
		       obj_size;

private int            alive,
                       obj_light,
                       obj_value,
                       *obj_coords;
private string         main_name,    /* main object name */
                       main_pname,   /* main plural name */
                       obj_short,    /* Short description */
                       obj_pshort,   /* Plural short description */
                       obj_long,     /* Long description */
                       *init_func,   /* funcs to call for reinitial. data */
                       *reload_func; /* funcs to query for reload data */
private object         environment;
private mapping        obj_data;     /* name(s), pname(s), adjective(s) */

#include "object/item.c"

/*
 * Function name: create_object
 * Description:   redefineable constructor function
 */
static void
create_object()
{
}

/*
 * Function name: create
 * Description:   Object constructor, called directly after load / clone
 */
static nomask void
create()
{
    init_props();
    obj_long = "";
    obj_data = ([ ]);
    add_prop(LIVE_BUTCHERED, -1);
    add_prop(LIVE_SKINNED, -1);
    create_object();
    
    "/base/debug"->debug_object(this_object());
}

nomask int
_Q_alive()
{
    return alive;
}

/*
 * Function name: enable_commands
 * Description:   mark this object as living
 */
static nomask void
enable_commands()
{
    alive = 1;
    if (!environment)
	return;
    /* change inventory position to 'living' */
    rlimits(10; 30000)
    {
	environment->_F_rm_inv(this_object());
	environment->_F_add_inv(this_object());
    }
}

/*
 * Function name: disable_commands
 * Description:   mark this object as non-living
 */
static nomask void
disable_commands()
{
    alive = 0;
    if (this_player() == this_object()) 
	set_this_player(0);
    if (!environment)
	return;
    /* change inventory position to 'non living' */
    rlimits(10; 30000)
    {
	environment->_F_rm_inv(this_object());
	environment->_F_add_inv(this_object());
    }
}

/*
 * Function name: environment
 * Description:   give surrounding container of this object
 * Returns:       object, 0 if no environment
 */
nomask object
environment()
{
    return environment;
}

/*
 * Function name: long
 * Description:   Describe the object
 * Returns:	  string, long description
 */
string
long()
{
    return obj_long;
}

/*
 * Function name: short
 * Description:   Return the short description of the object.
 * Returns:       string, short description
 */
string
short()
{
    if (query_prop(OBJ_INVIS))
	return "(" + obj_short + ")";
    return obj_short;
}

/*
 * Function name: plural_short
 * Description:   Return the plural short description of the object.
 * Returns:       string, plural short description
 */
string
plural_short()
{
    return obj_pshort;
}

/*
 * Function name: set_lwvs
 * Description:   sets the light, weight, volume and size value of this object
 * Arguments:     l - the new light value (inch how far to see)
 *                w - the new weight value (in gram)
 *                v - the new volume value (in milliliter)
 *                s - the new size value (in inch)
 */
nomask void
set_lwvs(int l, int w, int v, int s)
{
    if (environment)
	environment->update_internal(l - obj_light,
				     w - obj_weight,
				     v - obj_volume,
				     s - obj_size);
    obj_light  = l;
    obj_weight = w;
    obj_volume = v;
    obj_size   = s;
}

/*
 * Function name: add_lwvs
 * Description:   add a light, weight, volume and size value to the existing
 * Arguments:     l - the light value to add (inch how far to see)
 *                w - the weight value to add (in gram)
 *                v - the volume value to add (in milliliter)
 *                s - the size value to add (in inch)
 */
nomask void
add_lwvs(int l, int w, int v, int s)
{
    if (environment)
	environment->update_internal(l, w, v, s);
    obj_light  += l;
    obj_weight += w;
    obj_volume += v;
    obj_size   += s;
}

/*
 * Function name: query_light
 * Description:   gives the light value of this object
 * Returns:       integer, lightvalue
 */
int
query_light()
{
    return obj_light;
}

/*
 * Function name: query_weight
 * Description:   gives the weight value of this object
 * Returns:       integer, the weight
 */
int
query_weight()
{
    return obj_weight;
}

/*
 * Function name: query_volume
 * Description:   gives the volume value of this object
 * Returns:       integer, the volume
 */
int
query_volume()
{
    return obj_volume;
}

/*
 * Function name: query_size
 * Description:   gives the size value of this object
 * Returns:       integer, the size
 */
int
query_size()
{
    return obj_size;
}

/*
 * Function name: set_value
 * Description:   sets the new value of this object
 * Arguments:     v - the value (in smallest coin value)
 */
nomask void
set_value(int v)
{
    obj_value = v;
}

/*
 * Function name: query_value
 * Description:   gives the value of this object
 * Returns:       integer, the value
 */
int
query_value()
{
    return obj_value;
}

varargs void
remove_object(object ob)
{
    if ((!ob || ob == this_object()) && environment)
	environment->update_internal(-query_light(), -query_weight(),
				     -query_volume());
    ::remove_object(ob);
}

/*
 * Function name: id(string name)
 * Description:   check if the given name matches a name of this object
 * Arguments:     name - a string to compare with the object names
 * Returns:       1 if found, 0 else
 */
nomask int
id(string name)
{
    return (obj_data[name] & IS_NAME) != 0;
}

/*
 * Function name: query_data
 * Description:   collect all data of a given type
 * Arguments:     type - name, pname or adjective
 * Returns:       array of all collected data
 */
private string *
query_data(int type)
{
    int    i;
    string *ind;

    ind = map_indices(obj_data);
    if (!(i = sizeof(ind)))
	return ({ });
    while (i--)
	if (!(obj_data[ind[i]] & type))
	    ind[i] = 0;
    return ind - ({ 0 });
}

/*
 * Function name: add_data
 * Description:   add name, pname or adjective
 * Arguments:	  data - string(s) to add
 *		  type - type of data, name, pname or adjective
 */
private void
add_data(string data, int type)
{
    if (!data || !strlen(data))
	return;
    obj_data[data] |= type;
}

/*
 * Function name: del_data
 * Description:   remove name, pname or adjective
 * Arguments:	  data - string(s) to remove
 *		  type - type of data, name, pname or adjective
 */
private void
del_data(string data, int type)
{
    if (!data || !strlen(data))
	return;
    obj_data[data] -= (obj_data[data] & type);
}

/*
 * Function name: set_name
 * Description:   Sets main the name of the object
 * Arguments:	  name - the new name
 */
void
set_name(string name)
{
    add_data(name, IS_NAME);
    main_name = name;
    set_pname(LANG_PSENT(name));
}

/*
 * Function name: add_name
 * Description:   add name to the current names
 * Arguments:	  name - the additional name
 */
void
add_name(string name)
{
    add_data(name, IS_NAME);
    add_pname(LANG_PSENT(name));
}

/*
 * Function name: remove_name
 * Description:   Removes name(s)
 * Arguments:	  name - the name(s) to remove
 */
void
remove_name(mixed name)
{
    int i;
    
    if (!pointerp(name))
	del_data(name, IS_NAME);
    else
    {
	for (i=sizeof(name); i--;)
	    del_data(name[i], IS_NAME);
    }
}

/*
 * Function name: query_name
 * Description:   Gives the main name of the object
 * Returns:       string
 */
nomask string
query_name()
{
    return main_name;
}

/*
 * Function name: query_names
 * Description:   Gives the names of the object
 * Returns:       array
 */
string *
query_names()
{
    return query_data(IS_NAME);
}

/*
 * Function name: set_pname
 * Description:   Sets the main pluralname of the object
 * Arguments:	  name - the pname to add
 */
void
set_pname(string pname)
{
    add_data(pname, IS_PNAME);
    main_pname = pname;
}

/*
 * Function name: add_pname
 * Description:   adds the pluralform of a name 
 * Arguments:	  name - the pname to add
 */
void
add_pname(string pname)
{
    add_data(pname, IS_PNAME);
}

/*
 * Function name: remove_pname
 * Description:   Removes pname(s)
 * Arguments:	  pname - name(s) to remove
 */
void
remove_pname(mixed pname)
{
    int i;
    
    if (!pointerp(pname))
	del_data(pname, IS_PNAME);
    else
    {
	for (i=sizeof(pname); i--;)
	    del_data(pname[i], IS_PNAME);
    }
}

/*
 * Function name: query_pname
 * Description:   Gives the main plural name of the object
 * Returns:       string
 */
nomask string
query_pname()
{
    return main_pname;
}

/*
 * Function name: query_pname
 * Description:   Gives the plural names of the object
 * Returns:       array
 */
string *
query_pnames()
{
    return query_data(IS_PNAME);
}

/*
 * Function name: add_adj
 * Description:   add adjective
 * Arguments:	  adj - adjective to add
 */
void
add_adj(string adj)
{
    add_data(adj, IS_ADJ);
}

/*
 * Function name: remove_adj
 * Description:   Removes adjective(s)
 * Arguments:	  adj - adjective(s) to remove
 */
void
remove_adj(mixed adj)
{
    int i;
    
    if (!pointerp(adj))
	del_data(adj, IS_ADJ);
    else
    {
	for (i=sizeof(adj); i--;)
	    del_data(adj[i], IS_ADJ);
    }
}

/*
 * Function name: query_adjs
 * Description:   Gives the adjectives of the object.
 * Returns:       array, all adjectives
 */
string *
query_adjs()
{
    return query_data(IS_ADJ);
}


/*
 * Function name: set_ingredient
 * Description:   mark this object as an ingredient of the given type
 * Arguments:     type - type descriptor (see ingredients.h)
 */
nomask void
set_ingredient(string type)
{
    add_data(type, IS_INGREDIENT);
}

/*
 * Function name: query_ingredient
 * Description:   check if this is an ingredient of the given type
 * Arguments:     type - ingredient type (see ingredients.h)
 * Returns:       1 if this is such an ingredient, 0 else
 */
nomask int
query_ingredient(string type)
{
    return (obj_data[type] & IS_INGREDIENT) != 0;
}

/*
 * Function name: set_short
 * Description:   Sets the string to return for short description.
 * Arguments:     short - The short description
 */
void
set_short(string short)
{
    obj_short = short;
}

/*
 * Function name: set_pshort
 * Description:   Sets the string to return for plural short description.
 * Arguments:     pshort - The plural short description
 */
void
set_pshort(string pshort)
{
    obj_pshort = pshort;
}

/*
 * Function name: set_long
 * Description:   Sets the string to return as long description.
 * Arguments:     long - The long description
 */
nomask void
set_long(string long)
{
    obj_long = long;
}

/*
 * Function name: set_long_bs
 * Description:   Sets the string to return as long description and also calls
 *                break_string first.
 * Arguments:     long - The long description
 */
nomask void
set_long_bs(string long)
{
    obj_long = break_string(long, 75);
}

/*
 * Function name:   query_pronoun()
 * Description:     Returns the pronoun
 * Returns:         here: "it"
 */
string
query_pronoun()
{
    return "it";
}

/*
 * Function name:   query_possessive()
 * Description:     Returns the possessive
 * Returns:         here: "its"
 */
string
query_possessive()
{
    return "its";
}

/*
 * Function name:   query_objective()
 * Description:     Returns the objective
 * Returns:         here: "it"
 */
string
query_objective()
{
    return "it";
}

/*
 * Function name: set_coordinates
 * Description:   set the coordinates for this object.
 *                Remember: coordinates have to be set before a move
 *                          if the destination is a room.
 * Arguments:     c - the coordinates, array of x,y,z
 */
nomask void
set_coordinates(int *c)
{
    object surface;
    
    if (sizeof(c) != 3)
    {
	obj_coords = 0;
    }
    else
    {
	obj_coords = c[..];
    }
}

/*
 * Function name: query_coordinates
 * Description:   gives the coordinates of this object
 * Arguments:     co - optional player coordinates to determine correct
 *                     blocking distance
 * Returns:       the coordinates
 */
nomask varargs int *
query_coordinates(int *co)
{
    int *result, *bco;
    
    if (!co || !(bco = this_object()->query_range()))
	return obj_coords;

    result = obj_coords[..];
    if (co[0] >= obj_coords[0])
    {
	if (co[0] <= obj_coords[0] + bco[0])
	    result[0] = co[0]; /* inside object */
	else
	    result[0] += bco[0];
    }
    if (co[1] >= obj_coords[1])
    {
	if (co[1] <= obj_coords[1] + bco[1]) 
	    result[1] = co[1]; /* inside object */
	else
	    result[1] += bco[1];
    }
    if (co[2] >= obj_coords[2])
    {
	if (co[2] <= obj_coords[2] + bco[2]) 
	    result[2] = co[2]; /* inside object */
	else
	    result[2] += bco[2];
    }
    return result;
}

/*
 * Function name: parse_object
 * Description:   this function is called by parse(), check if this object
 *                matches the command string
 * Arguments:     cmd - array of exploded cmd string 
 *                type - if 1 match singular name,
 *                       if 2 match plural name,
 *                       if 3 match singular or plural name
 *                co - if not zero, include distance check
 *                d - if not zero, include distance check
 * Returns:       found name type
 */
varargs int
parse_object(string *cmd, int type, int *co, int d)
{
    int    i, j, t;
    string name, adj;

    if (co && d && environment->query_prop(IS_ROOM) &&
	!distance(co, query_coordinates(co), d))
	return 0;
    
    for (i=sizeof(cmd), name=""; i--;)
    {
	name = cmd[i] + name;
	if (t = obj_data[name] & type)
	{
	    for (j=i, adj=""; j--;)
	    {
		adj = cmd[j] + adj;
		if (obj_data[adj] & IS_ADJ)
		    adj = "";
		else
		    adj = " " + adj;
	    }
	    if (adj == "")
		return t;
	}
	name = " " + name;
    }
    return 0;
}

/*
 * Function name: check_audible
 * Description:   additional function to can_hear() within living.c
 *                (to improve speed)
 * Arguments:     dist - the maximum distance noise can be heard
 *                co - coordinates of listening player
 * Returns:       1 if visible, 0 else
 */
nomask int
check_audible(int dist, int *co)
{
    if (!distance(co, query_coordinates(),
     	    	  dist - query_prop(LIVE_NOISE_REDUCTION)))
	return 0;
    return 1;
}

/*
 * Function name: check_visible
 * Description:   additional function to visible() within living.c
 *                (to improve speed)
 * Arguments:     ldist - the maximum distance lighted
 *                see_invis - the level of invisible a living sees
 *                percep - perception of looking player
 *                co - coordinates of looking player
 *                env - environment of looking player
 * Returns:       1 if visible, 0 else
 */
nomask int
check_visible(int ldist, int see_invis, int percep, int *co, object env)
{
    int dist; /* maximum distance something can be seen */

    dist = query_size() * 100;
    if (!query_light())
    {
	if (!ldist) /* no light at all */
	    return 0;
	/* if the object is farther away than 'light' shines and
	 * carries no light, the view distance is reduced to 'ldist'
	 */
	if (dist > ldist)
	{
	    /* if any light, minimum sight distance is 80 inch */
	    if (ldist < 80) 
		dist = 80;
	    else
		dist = ldist;
	}
    }
    if (!distance(co, query_coordinates(), dist))
	return 0;
    if (percep < query_prop(OBJ_HIDE))
	return 0;
    if (query_prop(OBJ_INVIS) > 0 &&
	(see_invis < 1 || query_prop(OBJ_INVIS) > 999))
	return 0;
    return env->exit_visible(co, query_coordinates(), environment);
}

/*
 * Function name: add_reload
 * Description:   add a function to restore/save data
 * Arguments:     reload - the function to query for the reload data
 *                reinit - the function to call for setting the data
 */
nomask void
add_reload(string reload, string init)
{
    reload_func = reload_func ? (reload_func + ({ reload })) : ({ reload });
    init_func = init_func ? (init_func + ({ init })) : ({ init });
}

/*
 * Function name: reload_data
 * Description:   give an array of all data that must be restored after
 *                reloading the object (after new login or reboot)
 * Returns:       array
 */
nomask mixed *
reload_data()
{
    int    i, sz;
    object to;
    mixed  reload_data;
    
    if (!(sz = sizeof(reload_func)))
    {
	if (!obj_coords)
	    return 0;
	return ({ obj_coords });
    }
    reload_data = allocate(sz+1);
    reload_data[0] = obj_coords;
    to = TO();
    for (i=0; i<sz; i++)
	reload_data[i+1] = call_other(to, reload_func[i]);
    return reload_data;
}

/*
 * Function name: init_reload
 * Description:   initialize stored data after reload (see reload_data())
 * Arguments:     reload_data - the values to restore
 */
nomask void
init_reload(mixed reload_data)
{
    int    i, sz;
    object to;
    
    if (!sizeof(reload_data))
	return;
    if (sizeof(reload_data[0]) == 3)
	obj_coords = reload_data[0][..];
    if (!(sz = sizeof(init_func)))
	return;
    to = TO();
    for (i=0; i<sz; i++)
	call_other(to, init_func[i], reload_data[i+1]);
}

/*
 * Function name: attach_object
 * Description:   change the environment of an object, but insert into
 *                'attached' inventory
 * Arguments:     dest - destination object
 */
static nomask void
attach_object(object dest)
{
    int    i;
    object obj, e;

    if (!dest->_Q_inventory())
	error("No inventory at destination");
    if ((obj = this_object()) == dest)
	return;

    rlimits (10; 100000)
    {
	if (e = environment)
	    e->_F_rm_inv(obj);
	environment = dest;
    	dest->_F_attach(obj);
    }
    catch
    {
	if (e)
	    e->call_event(E_MOVE_INV, obj, e, dest);
	call_event(E_MOVE, obj, e, dest);
	dest->call_event(E_MOVE_INV, obj, e, dest);
    }
}

/*
 * Function name: move_object
 * Description:   change the environment of an object
 * Arguments:     dest - destination object
 */
static nomask void
move_object(object dest)
{
    object obj, e, surface;

    if ( !(dest->_Q_inventory()) )
	error("No inventory at destination.");
    if ((obj = this_object()) == dest)
    {
	error("Cannot put object into its own inventory.");
	return;
    }
    if (dest != environment)
    {
	rlimits (10; 100000)
	{
	    if (e = environment)
		e->_F_rm_inv(obj);
	    environment = dest;
	    dest->_F_add_inv(obj);
	}
	catch
	{
	    call_event(E_CHANGE_ENV, obj, e, dest);
	}
    }
    catch
    {
	if (e)
	    e->call_event(E_MOVE_INV, obj, e, dest);
	call_event(E_MOVE, obj, e, dest);
	dest->call_event(E_MOVE_INV, obj, e, dest);
    }
}

/*
 * Function name: attach
 * Description:   move this object to the attach inventory of another object
 * Arguments:	  dest - destination object 
 * Returns:       MOVE_OK     - Success.
 *	          MOVE_NODEST - No destination.
 */
nomask int
attach(object dest)
{
    object from;
    
    if (!dest)
	return MOVE_NODEST;
    obj_coords = 0; /* coordinates are not valid if attached */
    if (environment == dest)
    {
	/* moving inside an object */
	attach_object(dest);
	add_prop(OBJ_HIDE, 0); /* no more hidden after moving */
	add_prop(OBJ_HIDE_PLACES, 0);
    	return MOVE_OK;
    }
    from = environment;
    attach_object(dest);
    add_prop(OBJ_HIDE, 0); /* no longer hidden after moving */
    add_prop(OBJ_HIDE_PLACES, 0);
    
    if (from)
    {
	/* call remove_tool after moving, some event functions
	 * need to query the previously occupied slots
	 */
	if (living(from))
	    from->remove_tool(this_object(), 0); 
	from->update_internal(-query_light(), -query_weight(),
			      -query_volume());
    }
    if (dest)
	dest->update_internal(query_light(), query_weight(), 0);
    
    return MOVE_OK;
}

/*
 * Function name: move_now
 * Description:   Called by move, handles the update of light and weight/volume
 *                properties while moving this object.
 * Arguments:     dest - destination object
 *                from - where it comes from
 */
static nomask void
move_now(object dest, object from)
{
    move_object(dest);
    add_prop(OBJ_HIDE, 0); /* no longer hidden after moving */
    add_prop(OBJ_HIDE_PLACES, 0);
    
    if (from)
    {
	/* remove_tool after move_object, some event functions
	 * need to query the previously occupied slots 
	 */
	if (living(from))
	    from->remove_tool(this_object(), 0); 
	from->update_internal(-query_light(), -query_weight(),
			      -query_volume());
    }
    if (dest)
	dest->update_internal(query_light(), query_weight(), query_volume());
}

/*
 * Function name: move
 * Description:   Move this object to a new destination.
 *                Remember: coordinates have to be set before the move
 *                          if the destination is a room.
 * Arguments:	  dest - Object to move to
 * Returns:       (definitions see <move.h>)
 *                MOVE_OK     - Success.
 *	          MOVE_NODEST - No destination.
 *	          MOVE_NOGET  - The object can't be picked up.
 *	          MOVE_2HEAVY - The object is to heavy.
 *	          MOVE_2BIG   - The object is to big.
 *	          MOVE_NOCONT - The object is not a container.
 *	          MOVE_CATCHED - The move event was catched before moving
 */
nomask int
move(object dest)
{
    if (!dest)
	return MOVE_NODEST;
    
    if (call_event(E_MOVE_ATTEMPT, TO(), environment, dest) ||
	(dest && dest->call_event(E_MOVE_ATTEMPT, TO(), environment, dest)) ||
	(environment && environment->call_event(E_MOVE_ATTEMPT, TO(),
						environment, dest)))
	return MOVE_CATCHED;
	
    if (query_prop(IS_HEAP)) /* move_heap does the full moving */
	return this_object()->move_heap(dest);
    
    if (environment == dest)
    {
	/* moving inside an object */
	move_object(dest);
	add_prop(OBJ_HIDE, 0); /* no longer hidden after moving */
	add_prop(OBJ_HIDE_PLACES, 0);
    	return MOVE_OK;
    }
    if (dest->query_prop(IS_ROOM))
    {
	move_now(dest, environment);
	return MOVE_OK;
    }
    if (!dest->query_prop(IS_CONT))
	return MOVE_NOCONT;
    if (query_prop(OBJ_NO_GET))
	return MOVE_NOGET;
    if (dest->weight_left() < query_weight())
	return MOVE_2HEAVY;
    if (dest->volume_left() < query_volume())
	return MOVE_2BIG;
    move_now(dest, environment);
    obj_coords = 0; /* coordinates are not valid if not in room */
    return MOVE_OK;
}

