#include "../i2config.h"

#define WRAP(x)		sprintf("%-77s", x)
#define FROM		"frm"

nomask void
rcv_tell(mapping data)
{
    object ob;
    
    /* Compat with old-style SENDER handling. */
    if (data[SENDER])
	data[FROM] = data[SENDER];

    if (data[RECIPIENT] && (ob = find_player(lower_case(data[RECIPIENT]))) &&
	query_ip_number(ob))
    {
	if (!I2_DEMON->check_system_field(data, REPEAT))
	{
	    ob->catch_tell("\n" + WRAP(capitalize(data[FROM]) + "@" +
				       data[NAME] +
				       " tells you: " +
				       data[DATA]) + "\n");
	}
	I2_DEMON->send_udp(data[NAME],
			   ([
			       REQUEST: REPLY,
			       RECIPIENT: data[SENDER],
			       ID: data[ID],
			       DATA: ("You told " +
				      capitalize(data[RECIPIENT]) +
				      "@" + IMUD_MUDNAME + ": " +
				      data[DATA])
			       ]) );
	return;
    }
    /* data[SENDER] included for compat with old-style SENDER handling. */
    I2_DEMON->send_udp(data[NAME],
		       ([
			   REQUEST: REPLY,
			   RECIPIENT: data[SENDER],
			   ID: data[ID],
			   DATA: WRAP("Root@" + IMUD_MUDNAME +
				      ": No such player: " +
				      capitalize(data[RECIPIENT])) + "\n"
		 ]) );
}

nomask int
send_tell_req(string mud, string msg, string rcvname, object sender)
{
    if (previous_program() != _IMUDD)
	error("Illegal call to send_who_req");
    
    if (mud = I2_DEMON->send_udp(mud,
				 ([
				     REQUEST: "tell",
				     RECIPIENT: rcvname,
				     SENDER: sender->query_name(),
				     DATA: msg
				     ]), 1))
	sender->catch_tell(mud);
    else
	sender->catch_tell("Request transmitted.\n");
    return 1;
}
