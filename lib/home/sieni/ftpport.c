/*
 *  Muhveli MUD project - FTP port server daemon
 *
 */
 
#include <mudcon.h>
#include <network.h>

#define FTPCLIENT "/system/net/ftpcontrol"
#define MAX_FTP_SESSIONS 10

static int ourport; 
static int open;
object resolver;

int lastid;
mapping data;
mapping hosts;

int count;
int *lst;

int opentime;

int query_logintime()
{
    return opentime;
}

int query_port()
{
    return ourport;
}

void set_userid(string uid, int id)
{
    if(data[id])
    {
        data[id][1]=uid;
        if(data[id][0])
            data[id][0]->set_userid(uid);
    }
}

void set_hostname(string p1, string p2)
{
    int id;
    
    id=hosts[p2];
    if(data[id])
    {
        data[id][2]=p1;
        if(data[id][0])
            data[id][0]->set_hostname(p1);
    }
}

object connection(string host, int port)
{
    object ident;
    object client;
    
    client=clone_object(FTPCLIENT);
    ident=clone_object(IDENT);

    ++lastid;    
    ident->set_data(this_object(),host,port,ourport,lastid);
    ident->execute_ident(host);
    
    if(!resolver) resolver=DRIVER->query_resolver();
    if(resolver)
    {
        hosts[host]=lastid;
        host=resolver->gethostname(host,"set_hostname");
    }
    
    data[lastid]=({ client, ident, host, port, time(), 0 });

    client->set_dataid(lastid);
    client->set_hostname(host);
    client->set_userid(port+"");

    DRIVER->stdout(dump_variable(client));
    return client; 
}

void open_service()
{
    opentime=time();
    ourport=PORT_FTP;
    open_port("tcp",ourport);
}

void open()
{
    open=1;
/*   DRIVER->pilvimsg("using port "+ourport+" (TCP)");   */
}

void close(int dstr)
{
    open=0;
/*   DRIVER->pilvimsg("down ("+dstr+")"); */
}

void create()
{
    data=([ ]);
    hosts=([ ]);
    lst=({ });
}

mapping query_data()
{
    return data;
}

mixed *data_for_id(int id)
{
    return data[id];
}

void unregister_session(int id)
{
    if(previous_object()==data[id][0])
    {
       count--;
       data[id]=0;
    }    
}

int  ok_to_connect(int id)
{
    if(previous_object()!=data[id][0])
        return 1;
        
    if(count>=MAX_FTP_SESSIONS)
        return 2;
    
    return 0;
}

/*
static void check_connections()
{
    int *sockets, i, limit;
    int pfd;

    sockets = keys( socket_info );
    i = sizeof( sockets );

//    
//     check control connections for idle users
//    
    while ( i-- ) {
	if ( !undefinedp(socket_info[ sockets[i]][LAST_DATA]) ) {
	    limit = time() - socket_info[ sockets[i]][IDLE_SETTING];
	    if ( socket_info[ sockets[i]][LAST_DATA] < limit ) {
		socket_write( sockets[i],
		      "421 Timeout: closing control session.\n" );
		logout( sockets[i] );
		lose_user( sockets[i] );
	    }
	}
    }

    sockets = keys( socket_info );
    i = sizeof( sockets );

    while ( i-- ) {
	if (!undefinedp( socket_info[ sockets[i] ][ PARENT_FD ] )) {
	    pfd = socket_info[ sockets[i] ][ PARENT_FD ];
	    if (undefinedp(socket_info[ pfd ])) {
#ifdef FILE_LOCKING
		if (!undefinedp(socket_info[ sockets[i] ][ TYPE ]) &&
		      socket_info[ sockets[i] ][ TYPE ] == DOWNLOAD ) {
		    flock_wrapper( socket_info[ sockets[i] ][ PATH ], F_UNLOCK,
			  sockets[i] );
#endif
		}
		lose_connection( sockets[i] );
	    }
	}
    }
    call_out( "check_connections", 3 * 60 );
}
*/
