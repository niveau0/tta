/*
 * This file shall allow placing of objects onto this object's
 * surface.
 *
 * Ideas:
 * - max. carry_weight (wieviel man draufstellen kann, bevor kaputt)
 * - was ist mit liegenden Sachen, wenn Tisch sich bewegt?
 */
inherit "/base/object";
inherit "/system/composite";

#include <event.h>
#include <macros.h>
#include <move.h>
#include <properties.h>

//prototyping for  object.c
void add_adj(string adj);
//string long();
int query_volume();
int query_size();

// prototyping for surface.c
void remove_placed_object(object ex_object);


private mapping placed_objects; // contains objects and the area they fill
private int max_surface, used_surface;

/*
 * Function name: create_surface
 * Description:   initializes all variables 
 */
void
create_surface()
{
    int volume, size;
    
    add_adj("free");
    placed_objects = ([ ]);
    
    // general assumption for initialisation
    if ( (volume = query_volume()) & (size = query_size()) )
	max_surface = query_volume()/query_size();
    else
	max_surface = 0;
}

/*
 * Function name: set_surface
 * Description:   set the amount of surface on the object
 * Arguments:     new_surface - the area of surface in squareinches
 */
void
set_surface(int new_surface)
{
    object to, temp_object;

    to = this_object();
    
    if (new_surface < 0)
	return;
    while (new_surface < used_surface)
    {
	temp_object = placed_objects[map_indices(placed_objects)[0]];
	// new surface too small for all placed objects
	// remove objects randomly until fit
	remove_placed_object(temp_object);
	tell_objects(LISTEN(E(to))-({ to, temp_object }),
		     "The"+temp_object->short()+"gets too small for the"+
		     to->short()+".\n", MSG_SEE, to);
    }
    max_surface = new_surface;
}

/*
 * Function name: long
 * Description:   adds info about the objects on surface to long-desc
 * Returns:       original long-desc plus desc of surface
 */
string
long()
{
    int number;
    object *objects;
    string surface_description;

    if( number = sizeof(objects = map_indices(placed_objects)) )
    {
	surface_description = "There "+(number > 1?"are ":"is ");
	surface_description += composite(objects, this_player());
	surface_description += " on it.\n";
	return ::long()+surface_description;
    }
    
    return ::long();
}

/*
 * Function name: query_placed_objects
 * Description:   returns an array of all objects on the surface
 * Returns:       array of placed objects
 */
object *
query_placed_objects()
{
    return map_indices(placed_objects);
}

/*
 * Function name: remove_placed_object
 * Description:   removes an object from the surface and frees the area
 * Arguments:     ex_object - object to remove
 */
void
remove_placed_object(object ex_object)
{
    if (!ex_object)
	return;
    used_surface += placed_objects[ex_object];
    placed_objects[ex_object] = 0;
    ex_object->add_prop(OBJ_LYING_ON, 0);
}

/*
 * Function name: query_max_surface
 * Description:   returns the amount of maximum free surface
 * Returns:       the max free area in squareinches
 */
int
query_max_surface()
{
    return max_surface;
}

/*
 * Function name: place_object
 * Description:   tries to place a new object onto the surface
 * Arguments:     new_object - the object to add
 * Returns:       1 - if successful, 0 else (in case of not enough area)
 */
int
place_object(object new_object)
{
    int area, size, *old_coord, *new_coord;

    if ( !environment()->query_prop(IS_ROOM) )
    {
	this_player()->catch_tell("You can't put anything on the "+
				  short()+" while it's been carried.\n", 1);
	return 0;
    }
    
    size = new_object->query_size();
    
    // compute the area this object will need
    // object will be placed on side according to size
    area = new_object->query_volume()/size;
    //if ()
    //{
    //}
    if ( (used_surface+area) > max_surface)
    {
	// not enough space to place object
	return 0;
    }
    
    // object gets inserted into placed objects list
    old_coord = new_object->query_coordinates();
    // object is lying ON surface -> increased height
    new_coord = query_coordinates();
    new_object->set_coordinates( ({ new_coord[0], new_coord[1],
					(new_coord[2]+query_size()) }));
    
    // object is moved to the environment of the surface
    // for example the room, a table stands in
    if ( new_object->move(environment()) != MOVE_OK )
    {
	new_object->set_coordinates(old_coord);
	return 0;
    }

    placed_objects[new_object] = area;
    new_object->add_prop(OBJ_LYING_ON, this_object());
    
    return 1;
}
