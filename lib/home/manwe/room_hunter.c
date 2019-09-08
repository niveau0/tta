inherit "/base/npc";

#include <macros.h>
#include <stdproperties.h>

static string *way;
static mapping visited;

void create_npc()
{
    set_name("roomhunter");
    set_race_name("alien");
    set_short("rocket shaped alien");
    set_stats(({ 100, 100, 100, 100, 100, 100 }));
    add_prop(CONT_I_HEIGHT, 95);
    add_prop(CONT_I_WEIGHT, 63000);
    
    if (IS_CLONE)
    {
	set_living_name("roomhunter");
	call_out("hunt", 5);
    }
    visited = ([ ]);
    way = ({ });
}

static void
hunt()
{
    int    i, flag;
    string err, log, path;
    object env;
    mixed  exit;

    if (!(env=environment()))
    {
	destruct_object();
	return;
    }
    
    visited[object_name(env)] = 1;
    exit = env->query_exit();
    for (i=flag=0; i<sizeof(exit); i+=3)
    {
	if (visited[exit[i]])
	    continue;
	path = object_name(env);
	log = exit[i] + "\n";
	if (err = catch(command(exit[i+1])))
	{
	    log += "   " + err + "\n";
	    visited[exit[i]] = 1;
	}
	if (!sizeof(way) || way[sizeof(way)-1] != path && environment() != env)
	    way += ({ path });
	
	flag = 1;
	break;
    }
    if (!flag)
    {
	int sz;
	
	if (!(sz = sizeof(way)))
	{
	    log_file("ROOM_HUNT", "No more rooms to visit.\n");
	    destruct_object();
	    return;
	}
	for (i=flag=0; i<sizeof(exit); i+=3)
	{
	    if (exit[i] != way[sz-1])
		continue;
	    log = "Returning to " + exit[i] + "\n";
	    if (err = catch(command(exit[i+1])))
		log += "   " + err + "\n";
	    if (object_name(environment()) != exit[i] &&
		move_living("X", way[sz-1]))
		move(way[sz-1]);
	    flag = 1;
	    break;
	}
	if (!flag)
	{
	    debug("manwe", way);
	    log_file("ROOM_HUNT", "Bye, failed to return.\n");
	    destruct_object();
	    return;
	}
	way = way[..sz-2];
    }
    call_out("hunt", 1);
    log_file("ROOM_HUNT", log);
}
