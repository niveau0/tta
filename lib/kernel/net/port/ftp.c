/*
 * FTP port server daemon
 */

#include <net.h>
#include <ftpd.h>

#define DATA_CLIENT 0
#define DATA_HOST   1
#define DATA_IP     2
#define DATA_PORT   3
#define DATA_TIME   4

static int     id, count;
static mapping data;

static nomask void
create()
{
    open_port("tcp", PORT_FTP);
    data = ([ ]);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_FTP +
	     " (ftpd) successfully opened.\n");
}

static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_FTP + " (ftpd) closed.\n");
}

nomask void
resolved_host(string host, int the_id)
{
    if (!data[the_id])
	return;
    data[the_id][DATA_HOST] = host;
    data[the_id][DATA_CLIENT]->set_host(host);
}

static nomask object
connection(string ipnum, int port)
{
    object client;
    
    message(ipnum + " connected on port " + PORT_FTP + 
	    " (from " + port + ").\n");
    
    client = clone_object(FTPCLIENT);
    seteuid("0", client); /* not logged in, security */
    id++;

    data[id] = ({ client, 0, ipnum, port, time(), 0 });

    /*_RESOLVE_P->query(ipnum, "resolved_host", id);*/
    resolved_host(query_ip_name(client), id);
    

    client->init_client(id, ipnum, port);
    client->set_host(ipnum); /* set hostname to ipnum if resolve fails */
    count++;
    
    return client; 
}


nomask mapping
query_data()
{
    return data;
}

nomask mapping
query_data_id(int id)
{
    return data[id];
}

nomask int
check_connect(int id)
{
    if (!data[id] || previous_object() != data[id][DATA_CLIENT])
        return 0;
    if (count >= MAX_FTP_SESSIONS)
        return 0;
    return 1;
}

nomask void
close_session(int id)
{
    if (!data[id] || previous_object() != data[id][DATA_CLIENT])
	return;
    count--;
    data[id] = 0;
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
