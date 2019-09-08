#include <filepath.h>

private int     id, callouts, maxrooms;
private mapping visited;

/*
 * NAME:	open()
 * DESCRIPTION:	start a user connection on this object
 */
static void
open()
{
    message("@SERVER_READY\n");
    maxrooms = 20;
}

/*
 * NAME:	close()
 * DESCRIPTION:	close the user connection on this object
 */
nomask void
close(int f)
{
    if (!f)
	destruct_object();
}

static nomask void
send_room_data(string path, int runid)
{
    int     i, *co, *size;
    string  out, file, *ind;
    object  room;
    mapping exits;
    
    callouts--;
    if (!visited[runid] || !(room = _ROOMD->load_room(path)))
	return;
    if (map_sizeof(visited) > maxrooms)
	return;
    visited[path] = 1;
    co = room->query_coordinates();
    size = room->query_room_size();
    out = co[0] + "#" + co[1] + "#" + co[2] + "#" +
	  size[0] + "#" + size[1];
    message("@room" + out + "\n");
    exits = room->query_exits();
    ind = map_indices(exits);
    for (i=sizeof(ind); i--;)
    {
	file = FPATH(path + "/..", ind[i]);
	if (!visited[file])
	{
	    callouts++;
	    call_out("send_room_data", 1, file, runid);
	}
    }
    if (callouts < 1)
	message("@update\n");
}

/*
 * NAME:	receive_message()
 * DESCRIPTION:	receive user input
 */
static nomask void
receive_message(string str)
{
    int    max;
    string path;
    
    if (!str)
	return;
    set_this_player(0);
    if (sscanf(str, "@max%d\n", max))
	maxrooms = max;
    else if (sscanf(str, "@path%s\n", path))
    {
	id++;
	callouts = 1;
	visited = ([ id : 1 ]);
	send_room_data(path, id);
	return;
    }
}
