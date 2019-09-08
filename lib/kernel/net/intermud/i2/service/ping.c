#include "../i2config.h"

#define PING_TOTAL		0
#define PING_SENT		1
#define PING_CURRENT		2
#define PING_RECEIVED		3
#define PING_MIN_TIME		4
#define PING_AVG_TIME		5
#define PING_MAX_TIME		6
#define PING_LOST_PACKETS	7

#define PING_DATA_SIZE		8

#define PING_MAX_REQS		100
#define PING_DEFAULT_REQS	10

private mapping pings;

nomask string
get_ping_mud(string name)
{
    int    i;
    string *muds;
    
    for(i=sizeof(muds=map_indices(pings)); i--;)
    {
	if (pings[muds[i]][name])
	    return muds[i];
    }
}

nomask void
end_ping(string name, string mudname)
{
    object ob;
    mixed  *data;

    if (!pings[mudname] || !(data = pings[mudname][name]))
	return;
    if (ob = find_player(name))
    {
	if (!data[PING_CURRENT])
	    data[PING_CURRENT] = 1;
	if (data[PING_RECEIVED])
	    data[PING_RECEIVED] = 1;
	if (data[PING_LOST_PACKETS])
	    data[PING_LOST_PACKETS] = 1;
	
	ob->catch_tell("----" + capitalize(mudname) +
		       " PING Statistics----\n" +
		       data[PING_CURRENT] + " requests transmitted, " +
		       data[PING_RECEIVED] + " replies received (" +
		       (data[PING_RECEIVED] * 100 /
			data[PING_CURRENT]) + "%), " +
		       (data[PING_LOST_PACKETS] * 100 /
			(data[PING_RECEIVED] + data[PING_LOST_PACKETS]+1)) +
		       "% packet loss\nround-trip (s)  min/avg/max = " +
		       data[PING_MIN_TIME] + "/" +
		       floor((float)data[PING_AVG_TIME] + 0.5) +
		       "/" + data[PING_MAX_TIME] + "\n");
    }
    pings[mudname][name] = 0;
    if (!map_sizeof(pings[mudname]))
	pings[mudname] = 0;
}

nomask void
rcv_ping(mapping data)
{
    I2_DEMON->send_udp(data[NAME],
		       ([
			   REQUEST: REPLY,
			   RECIPIENT: data[SENDER],
			   ID: data[ID],
			   DATA: IMUD_MUDNAME + " is alive.\n"
			   ]) );
}

nomask void
rcv_reply(mapping data)
{
    int     i;
    string  mud, *names;
    object  ob;
    mapping mud_data;
    mixed   *user_data;

    if (!data[NAME] || !pings)
	return;
    if (!(mud_data = pings[mud = data[NAME]]))
	return;
    for(i=sizeof(names = map_indices(mud_data)); i--;)
    {
	if (!(ob = find_player(names[i])))
	{
	    pings[mud][names[i]] = 0;
	    continue;
	}
	user_data = mud_data[names[i]];
	if (!I2_DEMON->check_system_field(data, TIME_OUT))
	{
	    if (!user_data[PING_MIN_TIME] ||
		data[RESPONSE_TIME] < user_data[PING_MIN_TIME])
		user_data[PING_MIN_TIME] = data[RESPONSE_TIME];
	    
	    if (data[RESPONSE_TIME] > user_data[PING_MAX_TIME])
		user_data[PING_MAX_TIME] = data[RESPONSE_TIME];
	    
	    user_data[PING_AVG_TIME] = (float)
				       ((float)user_data[PING_AVG_TIME] *
					(float)user_data[PING_RECEIVED] +
					(float)data[RESPONSE_TIME]) /
				       (float)++user_data[PING_RECEIVED];
	    
	    ob->catch_tell("Reply from " + data[NAME] + ": id=" +
			   data[ID] + ". packet_loss=" +
			   data[PACKET_LOSS] + ". time=" +
			   data[RESPONSE_TIME] + ". s\n");
	}
	else
	{
	    ob->catch_tell("Reply timed out from " + data[NAME] + ": id=" +
			   data[ID] + ". packet_loss=" +
			   data[PACKET_LOSS] + ".\n");
	    end_ping(names[i], mud);
	    continue;
	}
	
	user_data[PING_LOST_PACKETS] += data[PACKET_LOSS];
	if (++user_data[PING_CURRENT] >= user_data[PING_TOTAL])
	    end_ping(names[i], mud);
    }
}

nomask int
send_ping_req(string mud, int packets, object sender)
{
    string name;
    mixed  data;
    
    if (previous_program() != _IMUDD)
	error("Illegal call to send_who_req");
    if (!pings)
	pings = ([ ]);
    name = sender->query_name();
    if (packets < 1)
    {
	if (!(mud = get_ping_mud(name)))
	{
	    sender->catch_tell("You are not currently pinging.\n");
	    return -1;
	}
	end_ping(name, mud);
	return 1;
    }
    if (packets > 1)
    {
	if (get_ping_mud(name))
	{
	    sender->catch_tell("You are already pinging a mud.\n");
	    return -1;
	}
	if (packets > PING_MAX_REQS)
	{
	    sender->catch_tell("Illegal number of packets.\n");
	    return -1;
	}
	if (!pings[mud])
	    pings[mud] = ([ ]);
	data = pings[mud][name] = allocate(PING_DATA_SIZE);
	data[PING_TOTAL] = packets;
	call_out("do_ping", 0, mud, name);
	sender->catch_tell("ping: Sending " + packets + " requests to " +
			   capitalize(mud) + ".\n");
	return 1;
    }
    if (name = I2_DEMON->send_udp(mud,
				  ([
				      REQUEST: PING,
				      SENDER: name,
				      ]), 1))
	sender->catch_tell(name);
    else
	sender->catch_tell("Request transmitted.\n");
    return 1;
}

static nomask void
do_ping(string mud, string name)
{
    mixed *data;

    if (!pings[mud] || !pings[mud][name])
	return;
    data = pings[mud][name];
    if (data[PING_SENT] < data[PING_TOTAL])
    {
	data[PING_SENT]++;
	I2_DEMON->send_udp(mud,
			   ([ REQUEST: PING, SENDER: this_object(), ]), 1);
	call_out("do_ping", 1, mud, name);
    }
}
