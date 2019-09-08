/* mud authorisation answer */
static nomask void
rcv_auth_mud_reply(mixed packet)
{
}

/* mud authorisation request */
static nomask void
rcv_auth_mud_req(mixed packet)
{
}

/* channel user request answer */
static nomask void
rcv_chan_user_reply(mixed packet)
{
}

/* channel user request */
static nomask void
rcv_chan_user_req(mixed packet)
{
}

/* channel who request answer */
static nomask void
rcv_chan_who_reply(mixed packet)
{
}

/* channel who request */
static nomask void
rcv_chan_who_req(mixed packet)
{
}

/* channellist request answer */
static nomask void
rcv_chanlist_reply(mixed packet)
{
    int     i;
    string  *ind;
    mapping list;
    
    if (sizeof(packet) != 8) 
	LOG("Bad chanlist-reply packet size");
    
    if (packet[PACKET_LOCALMUD] != RouterList[0][0])
	return;
    ChannelID = packet[6];
    ind = map_indices(list = packet[7]);
    for (i=sizeof(ind); i--;)
	ChannelList[ind[i]] = list[ind[i]];
}

/* channel target emote */
static nomask void
rcv_channel_e(mixed packet)
{
}

/* channel message */
static nomask void
rcv_channel_m(mixed packet)
{
}

/* channel emote */
static nomask void
rcv_channel_t(mixed packet)
{
}

/* error */
static nomask void
rcv_error(mixed packet)
{
    object tp;

    if (sizeof(packet) != 9) 
	LOG("Bad error packet size");
    if (!(tp = find_player(packet[PACKET_TARGETUSER])))
	return;
    tp->catch_tell("Intermud error: " + packet[7] + "\n");
}

/* finger request answer */
static nomask void
rcv_finger_reply(mixed packet)
{
    object tp;
    
    if (sizeof(packet) != 15) 
	LOG("Bad finger-reply packet size");
    if (!(tp = find_player(packet[PACKET_TARGETUSER])))
	return;
    smore("\n-------------------------------------------------------\n" +
	  packet[6]+"@"+packet[PACKET_LOCALMUD] +
	  (packet[7] ? "\nTitle: " + packet[7] : "") +
	  "\nLevel: " + packet[13] +
	  (packet[8] ? "\nRealname: " + packet[8] : "") +
	  (packet[9] ? "\nEmail: " + packet[9] : "") + 
	  (packet[11] < 0
	   ? ("\nLast login: " + packet[10] + 
	      "\n      from " + packet[12]) 
	   : "\nIdle time: " + packet[11]) + 
	  (packet[14]
	   ? ("\n-----------------------------------------------------\n" +
	      break_string(packet[14], 76, 1))
	   : "") +
	  "\n", tp);
}

/* finger request */
static nomask void
rcv_finger_req(mixed packet)
{
    string name, level, plan;
    object tp;
    
    if (sizeof(packet) != 7) 
	LOG("Bad finger-req packet size");
    if (!(tp = find_player(packet[6])))
	return;
    name = tp->query_name();
    switch (_ACCESS_LEVEL(name))
    {
      case _ADMIN: level = "Admin"; break;
      case _CREATOR: level = "Creator"; break;
      case _BUILDER: level = "Builder"; break;
      default: level = "Mortal"; break;
    }
    plan = _SYSTEMD->query_plan_file(name);
    
    send_data("finger-reply", 0, packet[PACKET_LOCALMUD],
	      packet[PACKET_LOCALUSER],
	      ({ capitalize(name), tp->query_presentation(tp),
		     0, tp->query_email(), tp->query_login_time(),
		     query_idle(tp), query_ip_number(tp), level,
		     plan }));
}

/* locate request answer */
static nomask void
rcv_locate_reply(mixed packet)
{
    object tp;
    
    if (sizeof(packet) != 10) 
	LOG("Bad locate-reply packet size");
    if (!(tp = find_player(packet[PACKET_TARGETUSER])))
	return;
    tp->catch_tell("Located " + packet[7] + " at " + packet[6] + ".\n" +
		   "Idle: " + packet[8] +
		   (packet[9] ? " Status: " + packet[9] : "") +
		   "\n");
}

/* locate request */
static nomask void
rcv_locate_req(mixed packet)
{
    int    idle;
    string status;
    object tp;
    
    if (sizeof(packet) != 7) 
	LOG("Bad locate-req packet size");
    if (!(tp = find_player(packet[6])))
	return;
    idle = query_idle(tp);
    if (tp->query_linkdead())
	status = "link-dead";
    else if (idle > 10*60)
	status = "inactive";
    else if (tp->query_prop(OBJ_INVIS) > 0)
	status = "invisible";
    else if (tp->query_prop(OBJ_HIDE) > 0)
	status = "hidden";
    
    send_data("locate-reply", 0, packet[PACKET_LOCALMUD],
	      packet[PACKET_LOCALUSER],
	      ({ IMUD_MUDNAME, tp->query_name(), idle, status }));
}

/* mudlist answer */
static nomask void
rcv_mudlist(mixed packet)
{
    int     i;
    string  *ind;
    mapping list;
    
    if (sizeof(packet) != 8) 
	LOG("Bad mudlist packet size");
    if (packet[6] == MudID)
	return; 
    if (packet[PACKET_LOCALMUD] != RouterList[0][0])
	return;
    MudID = packet[6];
    ind = map_indices(list = packet[7]);
    for (i=sizeof(ind); i--;)
    {
	MudList[ind[i]] = list[ind[i]];
	catch(_IMUDD->add_ip(ind[i], IMUD3));
    }
}

/* oob request */
static nomask void
rcv_oob_req(mixed packet)
{
}

/* startup request answer */
static nomask void
rcv_startup_reply(mixed packet)
{
    if (sizeof(packet) != 8)
	LOG("Bad startup-reply packet size");
    if (!sizeof(packet[6]))
	return;
    if (packet[6][0][0] == RouterList[0][0])
    {
	Password = packet[7];
	RouterList = packet[6];
    }
    else
    {
	RouterList = packet[6];
	RESTART_I3D;
	destruct_object();
    }
}

/* receive tell message */
static nomask void
rcv_tell(mixed packet)
{
    object tp;

    if (sizeof(packet) != 8)
	LOG("Bad tell packet size");
    if (!(tp = find_player(packet[PACKET_TARGETUSER])))
	return;
    tp->catch_tell(packet[6] + "@" + packet[PACKET_LOCALMUD] + ": " +
		   packet[7] + "\n");
}

/* user cache update */
static nomask void
rcv_ucache_update(mixed packet)
{
}

/* who request answer */
static nomask void
rcv_who_reply(mixed packet)
{
    int    i;
    string out;
    object tp;
    mixed  list;

    if (sizeof(packet) != 7)
	LOG("Bad who-reply packet size");
    if (!(tp = find_player(packet[PACKET_TARGETUSER])))
	return;
    list = packet[6];
    out = packet[PACKET_LOCALMUD] + " userlist\n";
    for (i=sizeof(list); i--;)
	out += sprintf("%-10s  (Idle)%-7d  %-50s\n",
		       list[i][0], list[i][1], list[i][2]+"");
    smore(out, tp);
}

/* who request */
static nomask void
rcv_who_req(mixed packet)
{
    int    i;
    string name, level;
    object *u;
    mixed  who_data;

    if (sizeof(packet) != 6)
	LOG("Bad who-req packet size");
    u = users();
    for (i=sizeof(u), who_data = allocate(i); i--;)
    {
	name = u[i]->query_name();
	switch (_ACCESS_LEVEL(name))
	{
	  case _ADMIN: level = "Admin"; break;
	  case _CREATOR: level = "Creator"; break;
	  case _BUILDER: level = "Builder"; break;
	  default: level = "Player"; break;
	}
	who_data[i] = ({ _SYSTEMD->query_coder_pretitle(name) + " " + capitalize(name), query_idle(u[i]), level });
    }
    send_data("who-reply", 0, packet[PACKET_LOCALMUD],
	      packet[PACKET_LOCALUSER], ({ who_data }));
}

nomask void
send_finger_req(string user, string mud, object sender)
{
    send_data("finger-req", sender->query_name(), mud, 0, ({ user }));
    sender->catch_tell("Request transmitted.\n");
}

nomask void
send_locate_req(string user, object sender)
{
    send_data("locate-req", sender->query_name(), 0, 0, ({ user }));
    sender->catch_tell("Request transmitted.\n");
}

nomask void
send_tell_req(string mud, string msg, string user, object sender)
{
    string name;
    
    name = sender->query_name();
    send_data("tell", name, mud, user, ({ capitalize(name), msg }));
    sender->catch_tell("Message transmitted.\n");
}

nomask void
send_who_req(string mud, object sender)
{
    send_data("who-req", sender->query_name(), mud, 0, ({ }));
    sender->catch_tell("Request transmitted.\n");
}
