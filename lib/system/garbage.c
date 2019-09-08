/*
 * garbage collector
 */
#include <config.h>
#include <macros.h>
#include <properties.h>

private string output;
private object next; /* next object in list */

nomask int
_Q_destruct()
{
    return _ADMIN;
}

static nomask void
create()
{
    call_out("run_garbage", GARBAGE_INTERVAL);
}

private int
check_room(object ob)
{
    string rfile;
    
    if (!ob->query_prop(IS_ROOM))
	return 0;
    
    if ((rfile = ob->query_room_file()) && file_size(rfile + ".o") > 0)
    {
	if (!ob->query_no_save() && strlen(rfile) > 4 &&
	    (rfile[..3] == "/d/" || rfile[..4] == "/home"))
	    catch(ob->save());
    }
    else
	rfile = 0;
    
    if (!rfile || !sizeof(I(ob, 5)))
    {
	output += "Remove room (" + (rfile ? rfile + ", " : "") +
		  object_name(ob) + ")";
	remove_object(ob);
	if (ob)
	    output += " --- Failed\n";
	else
	    output += " --- Done\n";
    }

    return 1;
}

private int
check_living(object ob)
{
    if (!ob->query_npc())
	return 0;
    if (!E(ob))
    {
	output += "Remove living (" + object_name(ob) + ")";
	remove_object(ob);
	if (ob)
	    output += " --- Failed\n";
	else
	    output += " --- Done\n";
    }
    return 1;
}

private int
check_object(object ob)
{
    string n;

    n = object_name(ob);
    if (strlen(n) > 4 && (n[..4] == "/lib/" || n[..4] == "/home") && !E(ob) &&
	program_name(ob) != n)
    {
	output += "Remove object (" + object_name(ob) + ")";
	remove_object(ob);
	if (ob)
	    output += " --- Failed\n";
	else
	    output += " --- Done\n";
    }
    return 1;
}

static nomask void
run_garbage()
{
    int    i;
    object n;

    call_out("run_garbage", GARBAGE_INTERVAL);

    if (!next && !(next = _DRIVER->_Q_next()))
	return;
    
    output = "";
    for (i=GARBAGE_OBJECTS; i--;)
    {
       	n = next->_Q_next();
	if (program_name(next) != object_name(next)) /* check only clones */
	{
	    if (!check_room(next))
		check_living(next);
	}
	if (!(next = n))
	    break;
    }
    if (strlen(output))
    {
	debug("manwe", "removed some garbage...");
	syslog(GARBAGE_LOG, output);
    }
    output = 0;
}
