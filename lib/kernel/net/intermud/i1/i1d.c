#include "i1config.h"

private mapping iplist,   /* holds all default udp-connections */
                channels; /* holds all local channel members */

#include "local.c"

static nomask int set_udp_status(string ipnum, int port, int status);
static nomask varargs void
rcv_ch_tell(string ipnum, int port, string ch, string msg, string name);

static nomask void
create()
{
    int    i;
    string *ind;
    
    /*    if (object_name(previous_object(2)) != _DRIVER)
	return;*/
    iplist = IPLIST;
    channels = ([ ]);
    local_init();
    
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	_IMUDD->add_ip(ind[i], IMUD1);
}

private string
convert_msg(string msg)
{
    if (!msg)
	return "";
    msg = implode(explode(msg, "@@"), "(at)(at)");
    msg = implode(explode(msg, "|"), "/");
    return msg;
}

nomask int
send_who_req(string ipname, object sender)
{
    string name;
    mixed  ipdata;

    if (!local_access(previous_object()) || !(ipdata = iplist[ipname]))
	error("Illegal call to send_who_req");
    name = lower_case(sender->query_name());
    IMUD1_PORT_OBJ->send_udp("@@" + RCV_WHO + "|" + name + "|" + "Q@@",
			     ipdata[LIST_IP], ipdata[LIST_PORT]);
    return 1;
}

nomask int
send_info_req(string ipname, object sender)
{
    string name;
    mixed  ipdata;
    
    if (!local_access(previous_object()) || !(ipdata = iplist[ipname]))
	error("Illegal call to send_info_req");
    name = lower_case(sender->query_name());
    IMUD1_PORT_OBJ->send_udp("@@" + RCV_INFO + "|" + name + "|" + "Q@@",
			     ipdata[LIST_IP], ipdata[LIST_PORT]);
    return 1;
}

static nomask void
channel_msg(string ch, string msg, string name)
{
    int    i;
    string *ind;
    
    msg = convert_msg(msg);
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	if (ind[i] == IMUD_MUDNAME)
	    rcv_ch_tell(UDP_LOCAL_IP, PORT_IMUD1_UDP, ch, msg, name);
	else
	    IMUD1_PORT_OBJ->send_udp("@@" + RCV_CH_TELL + "|" + ch + "|" +
				     msg + "|" + name + "@@",
				     iplist[ind[i]][LIST_IP],
				     iplist[ind[i]][LIST_PORT]);
}

nomask int
send_channel_req(string ch, string msg, object sender)
{
    string name;
    
    if (!local_access(previous_object()))
	error("Illegal call to send_channel_req");
    name = lower_case(sender->query_name());
    if (!channels[ch] || member_array(name, channels[ch]) < 0)
	return 0;
    channel_msg(ch, msg, name);
    return 1;
}

nomask int
send_tell_req(string ipname, string msg, string receiver, object sender)
{
    string name;
    mixed  ipdata;
    
    if (!local_access(previous_object()) || !(ipdata = iplist[ipname]))
	error("Illegal call to send_tell_req");
    msg = convert_msg(msg);
    name = lower_case(sender->query_name());
    receiver = lower_case(receiver);
    IMUD1_PORT_OBJ->send_udp("@@" + RCV_TELL + "|" + msg + "|" +
			     name + "|" + receiver + "@@",
			     ipdata[LIST_IP], ipdata[LIST_PORT]);
    return 1;
}

nomask int
send_join_req(string ch, object sender)
{
    string name;
    
    if (!local_access(previous_object()))
	error("Illegal call to send_join_req");
    name = lower_case(sender->query_name());
    if (!channels[ch])
	channels[ch] = ({ });
    else if (member_array(name, channels[ch]) > -1)
	return 0;
    channels[ch] |= ({ name });
    channel_msg(ch, capitalize(name) + " joined.", "control");
    return 1;
}

nomask int
send_leave_req(string ch, object sender)
{
    string name;
    
    if (!local_access(previous_object()))
	error("Illegal call to send_leave_req");
    name = lower_case(sender->query_name());
    if (!channels[ch])
	return 0;
    channel_msg(ch, capitalize(name) + " left.", "control");
    channels[ch] -= ({ name });
    return 1;
}

nomask int
send_list_req(string ipname, object sender)
{
    string name;
    mixed  ipdata;
    
    if (!local_access(previous_object()) || !(ipdata = iplist[ipname]))
	error("Illegal call to send_list_req");
    name = lower_case(sender->query_name());
    IMUD1_PORT_OBJ->send_udp("@@" + RCV_CH_LIST + "|" + name + "|" + "Q@@",
			     ipdata[LIST_IP], ipdata[LIST_PORT]);
    return 1;
}

/*****************************************************************************/

static nomask varargs void
rcv_ch_list(string ipnum, int port, string name, string data)
{
    object user;
    
    if (data == "Q")
    {
	data = local_ch_list(ipnum, port, name);
	if (!data || !strlen(data) || data == "Q")
	    /* Q is illegal as channel list -> infinite query */
	    return;
	data = convert_msg(data);
	IMUD1_PORT_OBJ->send_udp("@@" + RCV_CH_LIST + "|" + name + "|" +
				 data + "@@", ipnum, port);
	return;
    }
    if (!(user = USER(name)))
	return;
    MORE(user, data);
}

static nomask varargs void
rcv_ch_tell(string ipnum, int port, string ch, string msg, string name)
{
    int    i;
    string out, *members;
    object user;

    if (!(members = channels[ch]))
	return;
    
    out = local_ch_tell(ipnum, port, ch, msg, name);
    if (!out || !strlen(out))
	return;
    for (i=sizeof(members); i--;)
    {
	if (user = USER(members[i]))
	    TELL(user, out);
	else
	    channels[ch] -= ({ members[i] });
    }
    if (!sizeof(channels[ch]))
	channels[ch] = 0;
}

static nomask varargs void
rcv_tell(string ipnum, int port, string msg, string name, string receiver,
	 string ack)
{
    string out;
    object user;

    if (ack == "A")
    {
	out = local_tell_ack(ipnum, port, msg, name, receiver);
	if (!out || !strlen(out) || !(user = USER(name)))
	    return;
    }
    else
    {
	out = local_tell(ipnum, port, msg, name, receiver);
	if (!out || !strlen(out) || !(user = USER(receiver)))
	    return;
	out = convert_msg(out);
	/* send answer */
	IMUD1_PORT_OBJ->send_udp("@@" + RCV_TELL + "|" + msg + "|" +
				 name + "|" + receiver + "|A@@", ipnum, port);
    }
    TELL(user, out);
}

static nomask varargs void
rcv_who(string ipnum, int port, string name, string data)
{
    object user;
    
    if (data == "Q")
    {
	data = local_who(ipnum, port, name);
	if (!data || !strlen(data) || data == "Q")
	    /* Q is illegal as user list -> infinite query */
	    return;
	data = convert_msg(data);
	IMUD1_PORT_OBJ->send_udp("@@" + RCV_WHO + "|" + name + "|" +
				 data + "@@", ipnum, port);
	return;
    }
    if (!(user = USER(name)))
	return;
    MORE(user, data);
}

static nomask varargs void
rcv_info(string ipnum, int port, string name, string data)
{
    object user;
    
    if (data == "Q")
    {
	data = local_info(ipnum, port, name);
	if (!data || !strlen(data) || data == "Q")
	    /* Q is illegal as info -> infinite query */
	    return;
	data = convert_msg(data);
	IMUD1_PORT_OBJ->send_udp("@@" + RCV_INFO + "|" + name + "|" +
				 data + "@@", ipnum, port);
	return;
    }
    if (!(user = USER(name)))
	return;
    TELL(user, data);
}

static nomask varargs void
rcv_mail(string ipnum, int port, string name)
{
}

static nomask int
set_udp_status(string ipnum, int port, int status)
{
    int    i;
    string *ind;
    
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	if (iplist[ind[i]][LIST_IP] == ipnum &&
	    iplist[ind[i]][LIST_PORT] == port)
	{
	    iplist[ind[i]][LIST_STATUS] = status;
	    return 1;
	}
    return 0;
}

nomask int
receive_udp(string ipnum, string data, int p)
{
    int    type;
    string *args;

    if (!data || sscanf(data, "@@%s@@", data) != 1)
	return 0;
    /* set new timestamp for foreign port */
    if (!set_udp_status(ipnum, p, time()))
	return 0;
    args = explode(data, "|");
    sscanf(args[0], "%d", type);
    switch (type)
    {
      case RCV_CH_TELL:
	  rcv_ch_tell(ipnum, p, args[1..]...);
	  break;
      case RCV_TELL:
	  rcv_tell(ipnum, p, args[1..]...);
	  break;
      case RCV_WHO:
	  rcv_who(ipnum, p, args[1..]...);
	  break;
      case RCV_INFO:
	  rcv_info(ipnum, p, args[1..]...);
	  break;
      case RCV_MAIL:
	  rcv_mail(ipnum, p, args[1..]...);
	  break;
      case RCV_CH_LIST:
	  rcv_ch_list(ipnum, p, args[1..]...);
	  break;
    }
    return 0;
}

nomask mapping
query_iplist()
{
    return iplist;
}
