/*
 * let non living objects hear messages
 *
 * inherit this file
 * call start_listen to hear messages (see below)
 * call stop_listen to be deaf again
 * catch the message in catch_tell
 */

#include <event.h>
#include <macros.h>

void catch_tell(string msg);

static object listen_to;

/*
 * Function name: start_listen
 * Description:   begin listening
 * Arguments:     ob - optional object to listen to, defaults to this_object
 *                    (e.g. listen to messages which are heard by a coin
 *                     requires start_listen(coinobject))
 */
nomask varargs void
start_listen(object ob)
{
    object env;

    if (!ob)
	listen_to = this_object();
    else
	listen_to = ob;
    env = E(listen_to);
    while (living(env))
	env = E(env);
    add_event(E_MSG, "?", env);
    add_event(E_MOVE, "E_listen_move", listen_to);
}

nomask void
stop_listen()
{
    object env;

    if (!listen_to)
	return;
    env = E(listen_to);
    while (living(env))
	env = E(env);
    remove_event(E_MSG, "?", env);
    remove_event(E_MOVE, "E_listen_move", listen_to);
}

void
E_listen_move(object ob, object from, object to)
{
    if (ob != listen_to)
	return;
    while (living(from))
	from = E(from);
    remove_event(E_MSG, "?", from);
    
    while (living(to))
	to = E(to);
    add_event(E_MSG, "?", from);
}

nomask int
test_can_hear(object ob, int d)
{
    object *tar;
    
    if (!ob || !ob->check_audible(d, listen_to->query_coordinates()))
	return 0;
    return 1;
}

nomask varargs void 
catch_msg(mixed msg, object from, int type, int dist)
{
    int    i, sz;
    string str, name;
    mixed  sub;

    if (!(type & MSG_HEAR))
	return;
    if (from && !test_can_hear(from, dist))
	return;
    if (stringp(msg))
    {
	catch_tell(msg);
	return;
    }
    if (!pointerp(msg))
	error("Bad argument 1 for function catch_msg");
    str = "";
    for (i=0, sz=sizeof(msg); i<sz; i++)
    {
	if (stringp(sub = msg[i]))
	    str += sub;
	else
	{
	    if (sub[1])
		str += capitalize(sub[1]);
	    else
		str += sub[2];
	}
    }
    catch_tell(str);
}

nomask int
query_skill_rank(int skill)
{
    return 100; /* to understand all languages */
}
