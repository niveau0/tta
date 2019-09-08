#include <ftpd.h>
#include <net.h>

#define DOWNLOAD 1
#define UPLOAD   2

static int    dir, connected, pos, len, type, mode, append, passive;
static string command, data;
static object client;

nomask void
receive_data(object c, string path, string cmd,
	     int a, int m, int port, string ip)
{
    if (previous_program() != FTPCLIENT || !query_ip_number(previous_object()))
        error("Illegal call to receive_data");

    client  = c;
    data    = path;
    command = cmd;
    append  = a;
    mode    = m;
    dir     = DOWNLOAD;
    pos     = 0;
    connect(ip, port);
}

nomask void
send_data(object c, string str, string cmd,
	  int t, int m, int port, string ip)
{
    if (previous_program() != FTPCLIENT || !query_ip_number(previous_object()))
        error("Illegal call to send_data");

    client  = c;
    command = cmd;
    type    = t;
    mode    = m;
    dir     = UPLOAD;    
    pos     = 0;

    if (type == STRING) /* upload a string */
    {
	/* first replace \r characters so they won't be doubled */
	str = implode(explode("\r\n" + str + "\r\n", "\r\n"), "\n");
        data = implode(explode("\n" + str + "\n", "\n"), "\r\n");
    }
    else /* upload file */
        data = str;
    connect(ip, port);
}

private void
transfer() 
{
    int    length, startpos;
    string str;

    if (dir == DOWNLOAD)
        return;
    
    length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;
    
    startpos = pos;
    if (type == STRING)
    {
	pos += length;
	
	while (!send_message(data[startpos..pos-1]))
	{
	    startpos = pos;
	    if (startpos >= len)
		break;
            length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;
	    pos += length;
	}
    } 
    else
    {
        str = read_file(data, startpos, length);
	if (mode == ASCII)
	{
	    /* first replace \r characters so they won't be doubled */
	    str = implode(explode("\r\n" + str + "\r\n", "\r\n"), "\n");
	    str = implode(explode("\n" + str + "\n", "\n"), "\r\n");
	}
	pos += length;
	
	while (!send_message(str))
	{
	    startpos = pos;
	    if (startpos >= len)
		break;
            length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;	    
            str = read_file(data, pos, length);
	    if (mode == ASCII)
	    {
		/* first replace \r characters so they won't be doubled */
		str = implode(explode("\r\n" + str + "\r\n", "\r\n"), "\n");
		str = implode(explode("\n" + str + "\n", "\n"), "\r\n");
	    }
            pos += length;
	}
    }
    if (startpos >= len && !send_message(-1))
    {
	client->dataconn_tcomplete();
	destruct_object();
    }
}

static nomask void
open()
{
    connected = 1;
    
    if (type == FILE)
	len = file_size(data);
    else
	len = strlen(data);
    
    if (dir == UPLOAD)
    {
        client->dataconn_uopened(len, command, mode);
        transfer();
    }
    else if (dir == DOWNLOAD)
        client->dataconn_dopened(command, mode);
}

static nomask void
close(int flag) 
{     
    if (!connected)
    {
        if (client)
            client->dataconn_fail();
    }
    else if (dir == DOWNLOAD)
    {
        if (client)
            client->dataconn_tcomplete();
    }
    if (!flag)
	destruct_object();
}

/*nomask void
passive_mode()
{
    open_port("tcp", 3340);
}

static nomask object
connection(string ipnum, int p)
{
    _DRIVER->message(ipnum + " connected on port 1024 (from " + p + ").\n");

    return this_object();
}*/

static nomask void
receive_message(string str)
{
    if (dir == UPLOAD)
        return;
    set_this_player(0);

    if (mode == ASCII)
        str = implode(explode("\r\n" + str + "\r\n", "\r\n"), "\n");     
    
    if (append == NO_APPEND && !pos)
    {
	pos = 1; /* don't remove file again if more data arrives */
	remove_file(data);
    }
    write_file(data, str);
}

static nomask void
message_done()
{
    if (pos >= len)
    {
        client->dataconn_tcomplete();
        destruct_object();
        return;
    }
    transfer();
}

nomask object
ftpclient()
{
    return client;
}
