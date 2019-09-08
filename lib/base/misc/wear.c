/* inherit this module to make an object 'wearable' */

#include <combat.h>
#include <event.h>
#include <material.h>
#include <properties.h>

nomask void add_lwvs(int l, int w, int v, int s);

private int     saved_vol,   /* reduced volume while worn */
                layer,       /* which layer of slot is used */
                *race_sizes, /* min and max of wearers size */
                *locations;  /* Which locations the equipment should take */

private object  wearer;      /* Who is wearing it */

/*
 * Function name: set_layer
 * Description:   Set equipment layer. An object can be worn above other
 *                objects with a lower layer or above objects with the same
 *                layer, if the worn equipment is marked as thin (property
 *                OBJ_THIN_MATERIAL). Layer definitions see <slots.h>
 * Arguments:     l - LAYER_UNDERWEAR...Shirts, underwear
 *		      LAYER_CLOTH...pullovers, warm clothes
 *		      LAYER_CHAIN...jackets, things to wear over clothes but
 *		                    below thick armor and equipment
 *		      LAYER_PLATE...thick armor above other cloth/armor
 *		      LAYER_CAPE...above thick armor but below backpack
 *		      LAYER_ABOVE_ALL...backpacks, etc.
 */
static nomask void
set_layer(int l)
{
    layer = l;
}

/*
 * Function name: query_layer
 * Description:   gives the equipment layer. See also set_layer.
 * Returns:       integer
 */
nomask int
query_layer()
{
    return layer;
}

/*
 * Function name: set_locations
 * Description:   Set the locations the equipment uses
 * Arguments:     locs - integer array, the location-slots
 */
static nomask void
set_locations(int *locs)
{
    locations = locs;
}

/*
 * Function name: query_locations
 * Description:   Gives the locations which are used by this equipment
 * Returns:       integer array, the location-slots
 */
nomask int *
query_locations()
{
    return locations;
}

/*
 * Function name: set_wearer
 * Description:   Set the wearer of this object
 * Arguments:     The wearer object
 */
void
set_wearer(object w)
{
    /* not so much volume if worn */
    if (w && !wearer)
    {
	saved_vol = this_object()->query_volume() -
		    this_object()->query_volume() / 100;
	add_lwvs(0, 0, - saved_vol, 0);
	add_event(E_MOVE, "E_move_worn", this_object());
    }
    else if (!w && wearer)
    {
	add_lwvs(0, 0, saved_vol, 0);
	saved_vol = 0;
	remove_event(E_MOVE, "E_move_worn", this_object());
    }
    wearer = w;
}

/*
 * Function name: query_wearer
 * Description:   If this object is worn or not
 * Returns:       The object who wears this object if this object is worn
 */
nomask object
query_wearer()
{
    return wearer;
}

/*
 * Function name: set_race_sizes
 * Description:   set the min and max size of the allowed wearers
 * Arguments:     min - minimum size
 *                max - maximum size
 */
static nomask void
set_race_sizes(int min, int max)
{
    race_sizes = ({ min, max });
}

/*
 * Function name: query_race_sizes
 * Description:   gives min and max sizes wearer is allowed to have
 * Returns:       two elements array
 */
nomask int *
query_race_sizes()
{
    return race_sizes[..];
}

/*
 * Function name: E_move_worn
 * Description:   notify movement of this object, remove it if leaving wearer
 * Arguments:     ob - object that moved
 *                from - old environment
 *                to - new environment
 */
nomask void
E_move_worn(object ob, object from, object to)
{
    if (!wearer || ob != this_object() || wearer == to)
	return;
    set_wearer(0);
}

/*
 * Function name: wear_preposition
 * Description:   gives a prepostion for the wearing output
 * Returns:       default preposition string, "on"
 */
string
wear_preposition()
{
    return "on";
}
