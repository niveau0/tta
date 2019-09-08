#include <access.h>
#include <config.h>
#include <intermud.h>
#include <net.h>

#define MUDDATA_REALNAME 0
#define MUDDATA_IMUDID   1

private mapping mudlist;

static nomask void
create()
{
    mudlist = ([ ]);
    call_out("save", 1800);
    call_other(I1_DEMON, "???");
    call_other(I2_DEMON, "???");
    call_other(I3_DEMON, "???");
}

nomask void
save()
{
    call_out("save", 1800);
    I2_DEMON->save();
    I3_DEMON->save();
}

nomask void
add_ip(string name, int imudID)
{
    string lname, pobj;
    
    pobj = object_name(previous_object());
    if (pobj != I1_DEMON && pobj != I2_DEMON && pobj != I3_DEMON)
	error("Illegal call to add_ip");
    lname = lower_case(name);
    if (!mudlist[lname[0]])
	mudlist[lname[0]] = ([ ]);
    if (mudlist[lname[0]][lname])
	mudlist[lname[0]][lname][MUDDATA_IMUDID] |= imudID;
    else
	mudlist[lname[0]][lname] = ({ name, imudID, });
}

nomask void
remove_ip(string name, int imudID)
{
    string lname, pobj;
    
    pobj = object_name(previous_object());
    if (pobj != I1_DEMON && pobj != I2_DEMON && pobj != I3_DEMON)
	error("Illegal call to add_ip");
    lname = lower_case(name);
    if (!mudlist[lname[0]] || !mudlist[lname[0]][lname] ||
	!mudlist[lname[0]][lname][MUDDATA_IMUDID] & imudID)
	return;
    mudlist[lname[0]][lname][MUDDATA_IMUDID] ^= imudID;
}

static nomask string
resolveIp(string name)
{
    int     i;
    string  *ind;
    mapping muds;
    
    if (!name || !strlen(name))
	return 0;
    name = lower_case(name);
    if (!(muds = mudlist[name[0]]))
	return 0;
    if (muds[name])
	return name;
    ind = map_indices(muds);
    for (i=sizeof(ind); i--;)
	if (strlen(name) <= strlen(ind[i]) &&
	    name == ind[i][..strlen(name)-1])
	    return ind[i];
    return 0;
}

nomask int
query_mudlist(int imudID, object sender)
{
    string  out;
    
    if (imudID == 3)
	out = I3_DEMON->query_mudlist();
    else if (imudID == 2)
	out = I2_DEMON->query_mudlist();
    else if (imudID == 1)
	out = I1_DEMON->query_mudlist();
    else
	out = "No such intermud type: " + imudID + "\n";
    smore(out, sender);
    return 1;
}

nomask int
query_info(int imudID, string recvIp, object sender)
{
    string  out;
    
    if (imudID == 3)
	out = I3_DEMON->query_mudinfo(recvIp);
    else if (imudID == 2)
	out = I2_DEMON->query_mudinfo(recvIp);
    else if (imudID == 1)
	out = I1_DEMON->query_mudinfo(recvIp);
    else
	out = "No such intermud type: " + imudID + "\n";
    if (!out || !strlen(out))
	out = "Info not available.\n";
    smore(out, sender);
    return 1;
}

nomask int
send_channel_msg(string msg, string channel, string recvIp, object sender)
{
    if (!(recvIp = resolveIp(recvIp)))
	return -1;
}

nomask int
send_finger(string recvName, string recvIp, object sender)
{
    if (!(recvIp = resolveIp(recvIp)))
	return -1;
}


nomask int
send_join(string channel, object sender)
{
}

nomask int
send_leave(string channel, object sender)
{
}

nomask int
send_locate(string search, object sender)
{
}

nomask int
send_ping(int type, string recvIp, object sender)
{
    int   arg, imudID;
    mixed data;
    
    if (!recvIp)
    {
	sender->catch_tell("Bad mudname.\n");
	return -1;
    }
    if (sscanf(recvIp, "%s %d", recvIp, arg) != 2)
	arg = 1;
    if (!(recvIp = resolveIp(recvIp)))
    {
	sender->catch_tell("No such mud.\n");
	return -1;
    }
    data = mudlist[recvIp[0]][recvIp];
    imudID = data[MUDDATA_IMUDID];
    if (type == 2 && (imudID & IMUD2))
	I2_PING->send_ping_req(data[MUDDATA_REALNAME], arg, sender);
    /*else if (imudID & IMUD3)
    I3_DEMON->send_ping_req(data[MUDDATA_REALNAME], arg, sender);
    if ((imudID = data[MUDDATA_IMUDID]) & IMUD3)
    else if (imudID & IMUD1)
    I1_DEMON->send_ping_req(data[MUDDATA_REALNAME], sender);*/
    else
    {
	sender->catch_tell("Failed.\n");
	return -1;
    }
    return 1;
}

nomask int
send_tell(int type, string recvIp, string msg, string recvName, object sender)
{
    int   imudID;
    mixed data;
    
    if (!(recvIp = resolveIp(recvIp)))
	return -1;
    data = mudlist[recvIp[0]][recvIp];
    imudID = data[MUDDATA_IMUDID];
    if (type == 3 && (imudID & IMUD3))
	I3_DEMON->send_tell_req(data[MUDDATA_REALNAME], msg, recvName, sender);
    else if (type == 2 && (imudID & IMUD2))
	I2_TELL->send_tell_req(data[MUDDATA_REALNAME], msg, recvName, sender);
    else if (type == 1 && (imudID & IMUD1))
	I1_DEMON->send_tell_req(data[MUDDATA_REALNAME], msg, recvName, sender);
    else
    {
	sender->catch_tell("Failed.\n");
	return -1;
    }
    return 1;
}

nomask int
send_who(int type, string recvIp, object sender)
{
    int    imudID;
    mixed  data;
    
    if (!(recvIp = resolveIp(recvIp)))
	return -1;
    data = mudlist[recvIp[0]][recvIp];
    imudID = data[MUDDATA_IMUDID];
    if (type == 3 && (imudID & IMUD3))
	I3_DEMON->send_who_req(data[MUDDATA_REALNAME], sender);
    else if (type == 2 && (imudID & IMUD2))
	I2_WHO->send_who_req(data[MUDDATA_REALNAME], sender);
    else if (type == 1 && (imudID & IMUD1))
	I1_DEMON->send_who_req(data[MUDDATA_REALNAME], sender);
    else
    {
	sender->catch_tell("Failed.\n");
	return -1;
    }
    return 1;
}
