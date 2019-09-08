#include "../i2config.h"

nomask void
rcv_reply(mapping data)
{
    object ob;
    
    if (I2_DEMON->check_system_field(data, TIME_OUT))
    {
	if (data[SENDER])
	{
	    if (stringp(data[SENDER]) && (ob = find_player(data[SENDER])))
		ob->catch_tell("\n" + capitalize(data[REQUEST]) +
			       " request to " +
			       (data[RECIPIENT] ?
				capitalize(data[RECIPIENT]) +
				"@" + data[NAME] :
				data[NAME]) + " timed out.\n");
	    else if (objectp(ob = data[SENDER]) ||
		     (ob = find_object(data[SENDER])))
		call_other(ob, "rcv_reply", data);
	}
	return;
    }
    if (data[RECIPIENT])
    {
	/* If recipient is a player name, pass the message to them. */
	if (stringp(data[RECIPIENT]) &&
	    (ob = find_player(data[RECIPIENT])))
	    ob->catch_tell("\n" + data[DATA] + "\n");
	/* Otherwise send it to udp_reply() in the recipient. */
	else if (objectp(ob) || (ob = find_object(data[RECIPIENT])))
	    call_other(ob, "rcv_reply", data);
	return;
    }
}
