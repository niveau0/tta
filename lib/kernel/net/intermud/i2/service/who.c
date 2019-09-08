#include "../i2config.h"

nomask void
rcv_who(mapping data)
{
    int    i, sz;
    string out, n;
    object *u;

    u = users();
    out = "";
    for (i=0, sz=sizeof(u); i<sz; i++)
    {
	if (!(n = u[i]->query_name()))
	    continue;
	out += sprintf("  %s%s - %-25s\n",
		       (_ACCESS_LEVEL(geteuid(u[i])) > 1 ? "C" : " "),
		       (query_idle(u[i]) > 300 ? "I" : " "),
		       capitalize(n));
    }
    out = "User list of " + IMUD_MUDNAME +
	  "  ---  Flags: (C)oder, (I)dle\n" +
	  sprintf("%   *75s", out);

    I2_DEMON->send_udp(data[NAME],
		       ([
			   REQUEST: REPLY,
			   RECIPIENT: data[SENDER],
			   ID: data[ID],
			   DATA: out,
			   ]) );
}

nomask int
send_who_req(string mud, object sender)
{
    if (previous_program() != _IMUDD)
	error("Illegal call to send_who_req");
    
    if (mud = I2_DEMON->send_udp(mud, 
				 ([
				     REQUEST: "who",
				     SENDER: sender->query_name()
				     ]), 1))
	sender->catch_tell(mud);
    else
	sender->catch_tell("Request transmitted.\n");
    return 1;
}
