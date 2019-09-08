/*
 * UDP port handling code. Version 0.7beta2
 * Originally Written by Nostradamus for Zebedee.
 * Developed from an original concept by Alvin@Sushi.
 * Rewritten for DGD (TTAlib) by Manwe@tta
 */

#include "i2config.h"

nomask varargs string send_udp(string mudname, mapping data, int expect_reply);
nomask int check_system_field(mapping data, mixed value);

/* --- End of Config. Do not alter anything below. --- */

#define UNKNOWN		0
#define UP		time()
#define DOWN		(-time())

#define NEW_DELIMITER	"\n"
#define OLD_DELIMITER	"|"

#ifdef USE_OLD_DELIMITER
#define DELIMITER	OLD_DELIMITER
#else
#define DELIMITER	NEW_DELIMITER
#endif

#define HOSTFILE_DEL	":"
#define HOSTFILE_DEL2	","
#define RETRY		"_RETRY"

mapping hosts;

private int     packet_id;
private string  *received_ids;
private mapping pending_data, incoming_packets;

static nomask void
create()
{
    restore_object(I2_SAVEFILE);
    if (!hosts)
	hosts = ([ ]);
    packet_id = 0;
    pending_data = ([ ]);
    incoming_packets = ([ ]);
    received_ids = ({ });
    seteuid(_ROOT);
    call_out("startup", 1);
}

nomask void
save()
{
    save_object(I2_SAVEFILE);
}

/*
 * Make a startup PING request to all muds in the "hosts" mapping to set
 * HOST_STATUS information.
 */
static nomask void
startup()
{
    int    i;
    string *muds;

    for(i=sizeof(muds = map_indices(hosts)); i--;)
    {
	_IMUDD->add_ip(hosts[muds[i]][HOST_NAME], IMUD2);
	hosts[muds[i]][LOCAL_COMMANDS] = COMMANDS;
    }
    call_out("ping_muds", 1, muds);
}

static nomask void
ping_muds(string *muds)
{
    if (sizeof(muds))
    {
	  if (hosts[muds[0]])
	      send_udp(muds[0], ([ REQUEST: PING ]), 1);
	  call_out("ping_muds", 5, muds[1..]);
    }
    else
	  call_out("ping_muds", 60*30, map_indices(hosts));
}

nomask int
check_system_field(mapping data, mixed value)
{
    return data[SYSTEM] && (member_array(value, data[SYSTEM]) != -1);
}

static nomask mapping
add_system_field(mapping data, string field)
{
    if (data[SYSTEM])
	data[SYSTEM] |= ({ field });
    else
	data[SYSTEM] = ({ field });
    return data;
}

/*
 * Remove a buffered packet from the "incoming_packets" mapping.
 */
static nomask void
remove_incoming(string id)
{
    incoming_packets[id] = 0;
}

/*
 * Decode a string from a UDP packet.
 * Returns: The actual value of the argument (either int or string)
 */
static nomask mixed
decode(string arg)
{
    int i;
    
    if (!arg || arg=="")
	return 0;
    if (arg[0] == '$')
	return arg[1..];
    if (to_string(i = to_int(arg)) == arg)
	return i;
    return arg;
}

/*
 * Decode a UDP packet.
 * Arguments: UDP packet as a string.
 * Returns:   The decoded information as a mapping, 1 for success but no
 *            output (buffered packet), or 0 on error.
 */
static nomask mixed
decode_packet(string packet, string delimiter)
{
    int     i, id, n;
    string  info, tmp, *data;
    mapping ret;
    mixed   class;

    /* If this packet has been split, handle buffering. */
    if (packet[0..strlen(PACKET)] == PACKET + ":")
    {
	if (sscanf(packet, PACKET + ":%s:%d:%d/%d" + delimiter + "%s",
		   class, id, i, n, tmp) != 5)
	    return 0;
	class = lower_case(class) + ":" + id;
	if (pointerp(incoming_packets[class]))
	{
	    incoming_packets[class][i-1] = tmp;
	    if (member_array(0, incoming_packets[class]) == -1)
	    {
		ret = decode_packet(implode(incoming_packets[class], ""),
				    delimiter);
		incoming_packets[class] = 0;
		return ret;
	    }
	}
	else
	{
	    incoming_packets[class] = allocate(n);
	    incoming_packets[class][i-1] = tmp;
	    call_out("remove_incoming", INCOMING_TIME_OUT, class);
	}
	return 1;
    }
    
    ret = ([ ]);
    for(i=0, n=sizeof(data = explode(packet, delimiter)); i<n; i++)
    {
	/* DATA fields can be denoted by a preceeding blank field. */
	if (data[i] == "")
	{
	    tmp = DATA;
	    /* Test for illegal packet length (no DATA) */
	    if (++i >= n)
		return 0;
	    info = data[i];
	}
	else if (sscanf(data[i], "%s:%s", tmp, info) != 2)
	    return 0;
	switch((string)(class = decode(tmp)))
	{
	  case DATA:
	      return ret +
		  ([ DATA: decode(implode(
		      ({ info }) + data[i+1..], delimiter)) ]);
	  case SYSTEM:
	      ret[class] = explode(info, ":");
	      continue;
	  default:
	      ret[class] = decode(info);
	      continue;
	}
    }
    return ret;
}

/* Check wether a UDP packet was valid.
 * Logs are made and "host" information is updated as appropriate.
 * Arguments: Decoded UDP packet (mapping)
 * Returns:   0 for valid packets, an error string otherwise.
 */
static nomask string
valid_request_error(mapping data)
{
    int    i;
    string lname, req, *muds;
    mixed  host_data;
    
    if (!data[NAME] || !data[UDP_PORT])
	return "Illegal packet.\n";
    if (host_data = hosts[lname = lower_case(data[NAME])])
    {
	if (data[HOST] != host_data[HOST_IP])
	{
	    if (data[NAME] == IMUD_MUDNAME)
		return "*** FAKE MUD ***\n";
	    LOG("Host change:\n" + host_data[HOST_NAME] + ": " +
		host_data[HOST_IP] + " -> " + data[HOST] + "\n");
	    host_data[HOST_IP] = data[HOST];
	}
	if (data[UDP_PORT] != host_data[HOST_UDP_PORT])
	{
	    if (data[NAME] == IMUD_MUDNAME)
		return "*** FAKE MUD ***\n";
	    LOG("Port change:\n" + host_data[HOST_NAME] +
		" (" + host_data[HOST_IP] + "): " +
		host_data[HOST_UDP_PORT] + " -> " +
		data[UDP_PORT] + "\n");
	    host_data[HOST_UDP_PORT] = data[UDP_PORT];
	}
    }
    else
    {
	if (lname == lower_case(IMUD_MUDNAME))
	    return "*** FAKE MUD ***\n";
	for(i=sizeof(muds = map_indices(hosts)); i--;)
	{
	    host_data = hosts[muds[i]];
	    if (data[HOST] == host_data[HOST_IP] &&
		data[UDP_PORT] == host_data[HOST_UDP_PORT])
	    {
		LOG("Name change:\n" + host_data[HOST_NAME] + " (" +
		    host_data[HOST_IP] + ") -> " + data[NAME] + "\n");
		_IMUDD->remove_ip(host_data[HOST_NAME], IMUD2);
		_IMUDD->add_ip(data[NAME], IMUD2);
		host_data[HOST_NAME] = data[NAME];
		hosts[lname] = host_data;
		hosts[muds[i]] = 0;
		i = -2;
		break;
	    }
	}
	if (i != -2)
	{
	    _IMUDD->add_ip(data[NAME], IMUD2);
	    host_data = hosts[lower_case(data[NAME])] =
			({ PARSE_MUD_NAME(data[NAME]),
			   data[HOST],
			   data[UDP_PORT],
			   COMMANDS,
			   ({ "*" }),
			   UP
			});
	    LOG("New mud.\n" + data[NAME] + ":" + data[HOST] + ":" +
		data[UDP_PORT] + "\n");
	}
    }
    if (!(req = data[REQUEST]))
	return "System message.\n";
    if (req != PING && req != QUERY && req != REPLY &&
	member_array(req, host_data[LOCAL_COMMANDS]) == -1)
    {
	/* This should probably send a system message too. */
	send_udp(host_data[HOST_NAME], ([
	    REQUEST: REPLY,
	    RECIPIENT: data[SENDER],
	    ID: data[ID],
	    DATA: "Invalid request @" + IMUD_MUDNAME + ": " +
	    capitalize(data[REQUEST]) + "\n"
	    ]) );
	return "Invalid request.\n";
    }
    return 0;
}

/*
 * Incoming UDP packets are sent to this function to be interpreted.
 * The packet is decoded, checked for validity, HOST_STATUS is updated
 * and the appropriate udp module called.
 * Arguments: Senders IP address (string)
 *            UDP packet (string)
 */
nomask void
receive_udp(string sender, string packet)
{
    string lname, req, err, id;
    mixed  data;

    if (object_name(previous_object()) != IMUD2_PORT_OBJ)
	error("Illegal call to receive_udp");

    if (packet[strlen(packet)-1] == 0)
    {
	debug("manwe", "trailing 0: " + packet);
	packet = packet[..strlen(packet)-2];
	LOG("Received packet with trailing zero.\nSender: " +
	    sender + "\nPacket:\n" + packet + "\n");
    }
    
#ifdef DELIMITER_COMPAT
    data = decode_packet(packet, NEW_DELIMITER);
    
    /* Return if packet was buffered */
    if (data && !mappingp(data))
	return;
    
    /* If decode with NEW_DELIMITER failed or was invalid, and decode
     * with OLD_DELIMITER fails, return (possibly with error) */
    if ((!data || valid_request_error(data += ([ HOST: sender ]) )) &&
	!mappingp(data = decode_packet(packet, OLD_DELIMITER)))
#else
    /* If decode fails, return (possibly with error) */
	if (!mappingp(data = decode_packet(data, DELIMITER))) 
#endif
	{
	    /* Give an error if the packet wasn't buffered */
	if (!data)
#ifdef RECEIVE_UDP_COMPAT
	    RECEIVE_UDP_COMPAT(sender, packet);
#else
	    LOG("Received invalid packet.\nSender: " +
		sender + "\nPacket:\n" + packet + "\n");
#endif
	return;
    }
    
    data[HOST] = sender;
    if (err = valid_request_error(data))
    {
	LOG(err + "Sender: " + sender + "\nPacket:\n" + packet + "\n");
	return;
    }
    hosts[lname = lower_case(data[NAME])][HOST_STATUS] = UP;
    if ((req = data[REQUEST]) == REPLY)
    {
    	mapping pending;
	
	/* If we can't find the reply in the pending list then forget it. */
	if (!(pending = pending_data[lname + ":" + data[ID]]))
	    return;
	/* Set data[REQUEST] correctly, but still send to (req = REPLY) */
	data[REQUEST] = pending[REQUEST];
#ifdef DIAGNOSTICS
	data[PACKET_LOSS] = pending[PACKET_LOSS];
	data[RESPONSE_TIME] = time() - pending[RESPONSE_TIME] + 1;
#endif
	pending_data[lname + ":" + data[ID]] = 0;
    }
    else if (data[ID])
    {
	if (member_array(id = (lname + ":" + data[ID]), received_ids) == -1)
	{
	    received_ids += ({ id });
	    call_out("remove_received_id", RECEIVED_TIME_OUT, id);
	}
	else
	    add_system_field(data, REPEAT);
    }
    if (err = catch(call_other(I2_SERVICE + "/" + req, "rcv_" + req,
			       copy_mapping(data))))
    {
	send_udp(data[NAME], ([
	    REQUEST: REPLY,
	    RECIPIENT: data[SENDER],
	    ID: data[ID],
	    DATA: capitalize(req) + " request failed @" + IMUD_MUDNAME + ".\n"
	    ]) );
	LOG(data[REQUEST] + " from " + data[NAME] + " failed.\n" +
	    err + packet + "\n");
    }
}

nomask string
encode(mixed arg)
{
    if (objectp(arg))
	return object_name(arg);
    if (stringp(arg) &&
	(arg[0] == '$' || to_string(to_int(arg)) == (string)arg))
	return "$" + arg;
    return to_string(arg);
}

nomask string
encode_packet(mapping data)
{
    int    i, data_flag;
    string header, body, t1, t2, *ret;
    mixed  indices;

    for(ret=({ }), i=sizeof(indices=map_indices(data)); i--; )
    {
	if (indices[i] == DATA)
	{
	    data_flag = 1;
	    continue;
	}
	header = encode(indices[i]);
	body = encode(data[indices[i]]);
	if (sscanf(header, "%s:%s", t1, t2) ||
	    sscanf(header + body, "%s" + DELIMITER + "%s", t1, t2))
	    return 0;
	ret += ({ header + ":" + body });
    }
    if (data_flag)
#ifdef USE_OLD_DATA_FORMAT
	ret += ({ DATA + ":" + encode(data[DATA]) });
#else
	ret += ({ "", encode(data[DATA]) });
#endif
    return implode(ret, DELIMITER);
}

nomask string *
explode_packet(string packet, int len)
{
    if (strlen(packet) <= len)
	return ({ packet });
    return ({ packet[0..len-1] }) + explode_packet(packet[len..], len);
}

nomask varargs string
send_udp(string mudname, mapping data, int expect_reply)
{
    int    i;
    string packet, *packet_arr;
    mixed  host_data;

    mudname = lower_case(mudname);
    if (!(host_data = hosts[mudname]))
    {
	return "Unknown or ambiguous mudname: " +
	    capitalize(mudname) + "\n";
    }
    if (data[REQUEST] != PING && data[REQUEST] != QUERY &&
	data[REQUEST] != REPLY &&
	member_array("*", host_data[HOST_COMMANDS]) == -1 &&
	member_array(data[REQUEST], host_data[HOST_COMMANDS]) == -1)
	return capitalize(data[REQUEST]) + ": Command unavailable @" +
	    host_data[HOST_NAME] + "\n";
    
    data[NAME] = IMUD_MUDNAME;
    data[UDP_PORT] = PORT_IMUD2_UDP;
    if (expect_reply)
    {
	/* Don't use zero. */
	data[ID] = ++packet_id;
	pending_data[mudname + ":" + packet_id] =
#ifdef DIAGNOSTICS
	   data + ([ NAME: host_data[HOST_NAME], RESPONSE_TIME: time() ]);
#else
	   data + ([ NAME: host_data[HOST_NAME] ]);
#endif
    }
    if (!(packet = encode_packet(data)))
    {
	if (expect_reply)
	    pending_data[mudname + ":" + packet_id] = 0;
	LOG("Illegal packet sent by " + object_name(previous_object()) +
	    "\n");
	return "Illegal packet.\n";
    }
    if (expect_reply)
	call_out("reply_time_out", REPLY_TIME_OUT, mudname + ":" + packet_id);
    if (strlen(packet) <= MAX_UDP_PACKET_LEN)
	packet_arr = ({ packet });
    else
    {
	int    max;
	string header;
    
	/* Be careful with the ID.  data[ID] could have been set up by RETRY */
	header = PACKET + ":" + lower_case(IMUD_MUDNAME) + ":" +
	    ((expect_reply || data[REQUEST] != REPLY)&& data[ID] ?
	     data[ID] : ++packet_id) + ":";
	/* Allow 8 extra chars: 3 digits + "/" + 3 digits + DELIMITER */
	packet_arr = explode_packet(packet,
				    MAX_UDP_PACKET_LEN - (strlen(header) + 8));
	for(i=max=sizeof(packet_arr); i--; )
	    packet_arr[i] = header + (i+1) + "/" + max +
			    DELIMITER + packet_arr[i];
    }
    for(i=sizeof(packet_arr); i--;)
    {
	if (!IMUD2_PORT_OBJ->send_udp(packet_arr[i],
				      host_data[HOST_IP],
				      host_data[HOST_UDP_PORT]))
	    return "Error in sending packet.\n";
    }
    return 0;
}

static nomask void
reply_time_out(string id)
{
    string  lname;
    mapping data;
    
    if (data = pending_data[id])
    {
#ifdef DIAGNOSTICS
	data[PACKET_LOSS]++;
#endif
	if (data[RETRY] < RETRIES)
	{
	    mapping send;
	    
	    data[RETRY]++;
	    /* We must use a copy so the NAME field in pending_data[id]
	     * isn't corrupted by send_udp(). */
	    send = copy_mapping(data);
	    send[RETRY] = 0;
#ifdef DIAGNOSTICS
	    send[PACKET_LOSS] = 0;
	    send[RESPONSE_TIME] = 0;
#endif
	    call_out("reply_time_out", REPLY_TIME_OUT, id);
	    send_udp(data[NAME], send);
	    return;
	}
	data[RETRY] = 0;
#ifdef DIAGNOSTICS
	data[RESPONSE_TIME] = 0;
#endif
	catch(call_other(I2_SERVICE + "/" + REPLY, "rcv_reply",
			 add_system_field(data, TIME_OUT)));
	/* It's just possible this was removed from the host list. */
	if (hosts[lname = lower_case(data[NAME])])
	    hosts[lname][HOST_STATUS] = DOWN;
	remove_incoming(lname + ":" + id);
    }
    pending_data[id] = 0;
}

static nomask void
remove_received_id(string id)
{
    received_ids -= ({ id });
}

nomask varargs mixed
query(string what, mixed extra1, mixed extra2)
{
    mixed data;
    
    switch(what)
    {
      case "commands":
	  return COMMANDS;
      case "hosts":
	  return copy_mapping(hosts);
      case "pending":
	  return copy_mapping(pending_data);
      case "incoming":
	  return copy_mapping(incoming_packets);
      case "received":
	  return received_ids[..];
	  /* args: "valid_request", request, mudname */
      case "valid_request":
	  if (data = hosts[extra2])
	      return member_array("*", data[HOST_COMMANDS]) != -1 ||
		  member_array(extra1, data[HOST_COMMANDS]) != -1;
	  return 0;
    }
}

nomask string
query_mudlist()
{
    int    i, sz;
    string out, *ind;
    mixed  data;

    out = "Mudlist for Intermud2 connected muds\n";
    ind = map_indices(hosts);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	data = hosts[ind[i]];
	out += sprintf("%-20s  %-16s  %4s\n",
		       data[HOST_NAME],
		       data[HOST_IP],
		       (data[HOST_STATUS] > 0 ? "up" : "down"));
    }
    return out;
}
