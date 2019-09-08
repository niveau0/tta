#include <properties.h>

int type; /* room type from savefile to determine correct base file */

private mapping room_map;

static nomask void
create()
{
    room_map = ([ ]);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}

static nomask string *
split_path(string path)
{
    int    i, sz;
    string *split;

    split = allocate(2);
    
    for (i=strlen(path)-1; i>=0; i--)
    {
	if (path[i] == '/')
	{
	    split[0] = path[..i];
	    split[1] = path[i+1..];
	    break;
	}
    }
    return split;
}

nomask void
add_room(string path, object room)
{
    string *split;
    
    if (!room || !path)
	return;
    
    split = split_path(path);
    if (!mappingp(room_map[split[0]]))
	room_map[split[0]] = ([ split[1] : room ]);
    else
    {
	if (objectp(room_map[split[0]][split[1]]))
	    /* get rid of old room object if it is still present */
	    destruct_object(room_map[split[0]][split[1]]);
	room_map[split[0]][split[1]] = room;
    }
}

nomask void
remove_room(string path)
{
    string *split;

    if (!path)
	return;
    split = split_path(path);
    if (!mappingp(room_map[split[0]]))
	return;
    if (previous_object() != room_map[split[0]][split[1]])
	return;
    room_map[split[0]][split[1]] = 0;
    if (!map_sizeof(room_map[split[0]]))
	room_map[split[0]] = 0;
}

nomask mapping
query_room_map()
{
    return room_map;
}

nomask object
query_room(string path)
{
    string *split;

    if (!path)
	return 0;
    
    split = split_path(path);
    if (!mappingp(room_map[split[0]]))
	return 0;
    return room_map[split[0]][split[1]];
}

/*
 * Function name: load_room
 * Description:   load a room, connect it and store it inside the room map
 * Arguments:     path - the room path
 * Returns:       the room object-pointer
 */
nomask object
load_room(string path)
{
    int    l;
    string euid, *split;
    object ob;
    
    if (!path)
	return 0;
    split = split_path(path);
    if (mappingp(room_map[split[0]]) && (ob = room_map[split[0]][split[1]]))
	return ob;
    if (file_size(path + ".c") > 0)
    {
	catch(ob = compile_object(path));
	if (!ob || function_object("query_room_file", ob) != "/base/room")
	    return 0;
	ob->set_room_file(path);
	ob->load();
	if (!mappingp(room_map[split[0]]))
	    room_map[split[0]] = ([ split[1] : ob ]);
	else
	    room_map[split[0]][split[1]] = ob;
	return ob;
    }
    if (file_size(path + ".o") < 1)
	return 0;
    restore_object(path);
    if (type & TYPE_SHOP)
    {
	if (catch(ob = clone_object("/base/town/shop")))
	    return 0;
    }
    else
    {
	if (catch(ob = clone_object("/base/room")))
	    return 0;
    }
    ob->set_room_file(path);
    euid = geteuid(ob);
    if (catch(ob->load()))
    {
	destruct_object(ob);
	return 0;
    }
    if (!mappingp(room_map[split[0]]))
	room_map[split[0]] = ([ split[1] : ob ]);
    else
	room_map[split[0]][split[1]] = ob;
    return ob;
}
