inherit "/base/object";

#include <properties.h>
#include <material.h>
#include <combat.h>

static int     *range;
static mapping inside_wall;

static void
create_object()
{
    set_name("wall");
    set_name("water wall");
    set_short("water wall");
    set_pshort("water walls");
    
    add_prop(OBJ_MATERIAL, M_LIQUID);
    add_prop(OBJ_NO_GET, 1);
    add_lwvs(0, 28000000, 28000000, 120);
    inside_wall = ([ ]);
    add_event(E_MOVE, "E_move", this_object());
}

string
long()
{
    return "It's a " + (range[0] / 12) + "x" + (range[1] / 12) +
	" feet block of water.\n";
}

nomask int *
set_range(int *r)
{
    range = r;
}

nomask int *
query_range()
{
    return range;
}

nomask void
E_move(object ob, object from, object to)
{
    if (ob == this_object())
	add_event(E_MOVE_INV, "E_move_inv", to);
}

nomask void
E_move_inv(object ob, object from, object to)
{
    int *co, *wall_co;
    
    if (!ob || ob == this_object())
	return;

    wall_co = query_coordinates();
    co = ob->query_coordinates();
    range = ({ 120, 120, 120 });
    if (to != E(this_object()) || !in_range(co, wall_co, range))
    {
	if (inside_wall[ob])
	{
	    ob->catch_tell("You left the water.\n");
	    ob->cb_remove_boni(({ CB_ACTION_MOD }), ({ this_object() }));
	    inside_wall[ob] = 0;
	}
	return;
    }
    if (!inside_wall[ob])
    {
	ob->catch_tell("You enter the water cube. It's extremly hard " +
		       "to act.\n");
	ob->cb_add_boni(({ CB_ACTION_MOD }),
			({ ([ this_object():-60 ]) }));
	inside_wall[ob] = 1;
    }
}
