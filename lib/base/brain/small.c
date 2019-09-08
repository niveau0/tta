#include <macros.h>

private int running;

int    command(string cmd);
int    query_step();
int    query_size();
nomask varargs int *query_coordinates(int *co);
nomask object *visible(object *arr);

private int
check_direction(int index, int co, int step, int **coords)
{
    int i, sz, next, coord, diff;
    
    next = step + co;
    for (i=0, sz=sizeof(coords); i<sz; i++)
    {
	coord = coords[i][index];
	if ((diff = ABS(next - coord) - ABS(co - coord)) < 0 &&
	    ABS(diff) < 200)
	    break;
    }
    if (i == sz)
	return 1;
    return 0;
}

static nomask void
brain_do_flee()
{
    int    i, sz, step, coord, *co, **coords;
    object *liv;
    
    liv  = visible(I(E(this_object()), 2));
    if (!(sz = sizeof(liv)))
    {
	running = 0;
	return;
    }
    
    co   = query_coordinates();
    step = query_step();
    for (i=0, coords = allocate(sz); i<sz; i++)
	coords[i] = liv[i]->query_coordinates();

    if (check_direction(1, co[1], step, coords))
	command("north");
    else if (check_direction(1, co[1], -step, coords))
	command("south");
    else if (check_direction(0, co[0], step, coords))
	command("west");
    else if (check_direction(0, co[0], -step, coords))
	command("east");
    else
    {
	running = 0;
	return;
    }
    
    if (running++ < 5)
	call_out("brain_do_flee", 1);
    else
    {
	running = 0;
	return;
    }
}

nomask void
brain_flee(object ob)
{
    if (running || ob == this_object() || !living(ob) || ob->query_size() <
	2 * this_object()->query_size())
	return;
    brain_do_flee();
}
