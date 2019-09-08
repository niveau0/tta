inherit "convert";

#include <access.h>
#include <config.h>
#include <intermud.h>
#include <net.h>
#include <properties.h>
#include <status.h>
#include <type.h>

#define PACKET_TYPE       0
#define PACKET_TTL        1
#define PACKET_LOCALMUD   2
#define PACKET_LOCALUSER  3
#define PACKET_TARGETMUD  4
#define PACKET_TARGETUSER 5

int     MudID, ChannelID, Password;
mapping MudList, ChannelList;
mixed   **RouterList;

private string  *databuffer, **packet_queue;
private mapping Services;

private void
send_data(string type, string localuser, string targetmud,
	  string targetuser, mixed data)
{
    int   psize;
    mixed packet;

    packet = ({ type, 5, IMUD_MUDNAME, localuser,
		targetmud, targetuser }) + data;

    psize = strlen(packet = to_string(packet) + "\0");
    packet = "    " + packet;

    packet[0] = (psize & 0xff000000) >> 24;
    packet[1] = (psize & 0xff0000) >> 16;
    packet[2] = (psize & 0xff00) >> 8;
    packet[3] = psize & 0xff;

    send_message(packet);
/*    debug("manwe", "sent: "+packet);*/
}

#include "service.c"

/*
 * Function name: create
 * Description:   constructor of the intermud3 demon
 */
static nomask void
create()
{
    int    port;
    string ip;

    seteuid(_ROOT);
    
    Password = 0;
    MudID = 0;
    ChannelID = 0;

    restore_object(I3_SAVEFILE);
    if (!MudList)
	MudList = ([]);
    if (!ChannelList)
	ChannelList = ([]);
    if (!RouterList)
	RouterList = ({ ({ "*gjs", "198.144.203.194 9000" }) });
    
    databuffer = ({ });
    packet_queue = ({ });
        
    Services = ([
		 /*	"auth-mud-reply"  : "rcv_auth_mud_reply",
			"auth-mud-req"    : "rcv_auth_mud_req",
			"chan-user-reply" : "rcv_chan_user_reply",
			"chan-user-req"   : "rcv_chan_user_req",
			"chan-who-reply"  : "rcv_chan_who_reply",
			"chan-who-req"    : "rcv_chan_who_req",*/
	"chanlist-reply"  : "rcv_chanlist_reply",
	"channel-e"       : "rcv_channel_e",
	"channel-m"       : "rcv_channel_m",
	"channel-t"       : "rcv_channel_t",
	"error"           : "rcv_error",
	"finger-reply"    : "rcv_finger_reply",
	"finger-req"      : "rcv_finger_req",
	"locate-reply"    : "rcv_locate_reply",
	"locate-req"      : "rcv_locate_req",
	"mudlist"         : "rcv_mudlist",
	/*	"oob-req"         : "rcv_oob_req",*/
	"startup-reply"   : "rcv_startup_reply",
	"tell"            : "rcv_tell",
	/*	"ucache-update"   : "rcv_ucache_update",*/
	"who-reply"       : "rcv_who_reply",
	"who-req"         : "rcv_who_req",
	/*	"emoteto"         : "rcv_emoteto",*/
	]);

    call_out("reconnect", 1);
}

nomask void
save()
{
    save_object(I3_SAVEFILE);
}

/*
 * Function name: reconnect
 * Description:   try to reopen the connection
 */
static nomask void
reconnect()
{
    int    port;
    string ip;
    
    sscanf(RouterList[0][1], "%s %d", ip, port);
    connect(ip, port);
}

/*
 * Function name: open
 * Description:   Called if the tcp connect has been opened successfully.
 */
static nomask void
open()
{
    send_data("startup-req-3", 0, RouterList[0][0], 0,
	      ({ Password, MudID, ChannelID, PORT_TELNET,
		     PORT_IMUD3_TCP, PORT_IMUD3_UDP, MUDLIB_VERSION, 
		     MUDLIB_VERSION, "DGD v"+status()[ST_VERSION],
		     "LPC/DGD", "mudlib development", ADMIN_EMAIL,
		     ([
		       /* "amcp"    : AMCP_VERSION,
			  "auth"    : 1,
			  "channel" : 1,
			  "emoteto" : 1,
			  "file"    : 1,*/
			 "finger"  : 1,
			 "ftp"     : PORT_FTP,
			 "http"    : PORT_HTTP,
			 "locate"  : 1,
			 /* "mail"    : 1,
			  "news"    : 1,
			  "nntp"    : PORT_NNTP,*/
			 "tell"    : 1,
			 /* "ucache"  : 1,*/
			 "who"     : 1,
			 /* "rcp"     : PORT_RCP,
			 "smtp"    : PORT_SMTP,*/
			 ]),
		     ([]),
		     }));
    _DRIVER->message("Intermud3 router successfully connected.\n");
}

/*
 * Function name: close
 * Description:   Called if the connection closed. If this happens reopen()
 *                is tried once.
 */
static nomask void
close(int flag)
{
    _DRIVER->message("Intermud3 router connection closed.\n");
    call_out("reconnect", 5);
}

/*
 * Function name: process_data
 * Description:   handle a single packet
 * Arguments:     packet - the packet to process
 */
private void
process_data(mixed packet)
{
    string func;

    if (func = Services[packet[PACKET_TYPE]])
    {
	rlimits (20; 400000)
	{
	    call_other(this_object(), func, packet);
	}
    }
    else
    {
	LOG("received unsupported " + packet[PACKET_TYPE] + " request");
	/*	  send_data("error", 0,
		  packet[PACKET_LOCALMUD],
		  packet[PACKET_LOCALUSER],
		  ({ "unk-user", "Service not available.", packet }));*/
    }
}

/*
 * Function name: process_message
 * Description:   handle pending packets
 * Returns:       1 if there are still pending packets, 0 else
 */
static nomask int
process_message()
{
    string *packet;
    if (sizeof(packet_queue) < 1)
	return 0;
    packet = packet_queue[0];
    if (strlen(packet[0]) > 3)
	catch(process_data(to_value(packet)));

    packet_queue = packet_queue[1..];
    if (sizeof(packet_queue) > 0)
    {
        call_out("process_message", 3);
        return 1;
    }
    return 0;
}

/*
 * Function name: receive_message
 * Description:   called for incoming data
 * Arguments:     data - the current data packet
 * Returns:       0 if processed, 1 else
 */
static nomask int
receive_message(string data)
{
    int    len, i;
    string *split;

    set_this_player(0);

    split = explode(data, "})\0");

/*    debug("manwe", data);*/

    if (sizeof(split) > 0)
    {
        if (sizeof(databuffer) == 0)
	    databuffer += ({ split[0][4..] });
        else
	    databuffer += ({ split[0] });
    
	len = strlen(data);
	if (sizeof(split) > 1 || (len > 2 && data[len-3..] == "})\0")) /* found end of a packet */
        {
            databuffer += ({ "})" });
            packet_queue += ({ databuffer }); /* add first now finished packet */
            databuffer = ({ });
             
	    for (i=1; i<sizeof(split)-1; i++)
		packet_queue += ({ ({ split[i][4..]+"})" }) }); /* add all finished packets between first and last */
            
            if (i < sizeof(split))
            {
                if (data[len-3..] == "})\0")
  	            packet_queue += ({ ({ split[i][4..]+"})" }) }); /* add last, finished packet */
	        else
	            databuffer = ({ split[i][4..] }); /* store last unterminated packet */
            }
	    process_message();
	    return 1;
	}
    }
    return 0;
}


nomask string
query_mudlist()
{
    int    i, sz;
    string out, *ind;
    mixed  data;

    out = "Mudlist for Intermud3 connected muds\n";
    ind = map_indices(MudList);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	data = MudList[ind[i]];
	out += sprintf("%-20s  %-4s  %-15s - %s\n",
		       ind[i], (data[0] < 0 ? "up" : "down"),
		       data[7], data[10]);
    }
    return out;
}

nomask string
query_mudinfo(string name)
{
    string out;
    mixed  data;

    data = MudList[name];
    out = "Intermud 3 info for " + name + ":\n";
    if (!data)
	return out + "unknown\n";
    out += sprintf(" Ip:     %s:%d (%s)\n Driver: %s\n Type:   %s\n Lib:    %s, %s\n State:  %s\n Admin:  %s\n",
		       data[1], data[2], (data[0] < 0 ? "up" : "down"),
		       data[7], data[8], data[5],
		       data[9], data[6], data[10]);
    return out;
}

nomask mapping
query_channellist()
{
    return ChannelList;
}

nomask string *
query_channels()
{
    return map_indices(ChannelList);
}
