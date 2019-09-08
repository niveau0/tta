#include <net.h>

#define TIMEOUT        10 /* seconds */

private int     id;
private string  function;
private mapping asker;

static nomask void
create()
{
    asker = ([ ]);
    open_port("udp", PORT_RESOLVE);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_RESOLVE +
	     " (resolve) successfully opened.\n");
}

static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_RESOLVE + " (resolve) closed.\n");
}

private void
print_data(string data)
{
    int    i, j, d;
    string out, s;
    
    out = "\nHeader\n0  1  2  3  4  5  6  7 | 8  9 10 11 12 13 14 15\n\n";
    for (i=0; i<strlen(data); i++)
    {
	d = data[i];
	out += sprintf("%d  %d  %d  %d  %d  %d  %d  %d ",
		       (d&128?1:0), (d&64?1:0), (d&32?1:0), (d&16?1:0),
		       (d&8?1:0), (d&4?1:0), (d&2?1:0), (d&1?1:0)); 
	if (i%2)
	{
	    s[2] = (d<32 ? 32 : d);
	    out += s + "\n";
	}
	else
	{
	    s = "   ";
	    s[1] = (d<32 ? 32 : d);
	    out += "| ";
	}
	if (i == 11)
	    out += "\nQuestion\n";
    }
    debug("manwe", out + "\n");
}

nomask static void
timeout(int id)
{
    asker[id] = 0;
}

/*
 * Function name: receive_datagram
 * Description:   Called if some data has arrived at the udp port.
 * Arguments:     data - the arriving data
 *                ipnum - the ipnum where data is coming from
 *                p - the port where data is coming from
 * Returns:       0 if finished, 1 if process_datagram should be called
 *                as soon as possible
 */
static nomask int
receive_datagram(string data, string ipnum, int p)
{
    int    i, len, msgid;
    string header, s, out;

    if (!data || strlen(data) < 12)
	return 0;
    set_this_player(0);
    header = data[..11];
    if (!(header[2] & 128)) /* not a response */
	return 0;
    if (header[3] & 15) /* failure */
	return 0;
    if (header[7] < 1) /* no Answer count */
	return 0;
    msgid = (header[0] & 0x2f) + header[1];
    if (!asker[msgid] || !asker[msgid][0])
	return 0;
    
    i = 12;
    while (data[i++]); /* skip Question field */
    i += 4; /* skip class & type of question */

    if ((data[i] & 0xc0) == 0xc0) /* message starts with pointer */
	i += (data[i] & 0x2f) + data[i+1];

    out = "";
    s = " ";
    while (len = data[i++])
    {
	while (len--)
	{
	    s[0] = data[i++];
	    out += s;
	}
	if (!data[i])
	    break;
	out += ".";
    }
    call_other(asker[msgid][0], asker[msgid][1], out, asker[msgid][2]);
    return 0;
}

#define S16BITZERO "\0\0\0\0"
#define C2P(s) (a8bit2str(strlen(s)) + s)

private string
a16bit2str(int val)
{
    string s;
    
    s = "  ";
    s[0] = val >> 8;
    s[1] = val & 0xff;
    return s;
}

private string
a8bit2str(int val)
{
    string s;
    
    s = " ";
    s[0] = (val&0xff);
    return s;
}

private string
addr2qname(string address)
{
    int    i;
    string *quad_bits, qaddr;

    quad_bits = explode(address,".");
    qaddr = "";
    
    i=0;
    while (i<sizeof(quad_bits))
    {
	qaddr += C2P(quad_bits[i]);
	i++;
    }
    qaddr += "\0";
    return qaddr;
}

private void
_query(string ip)
{
    int    i;
    string header, qname, *split;

    if (!ip)
	return;
    
    if ((i=sizeof(split = explode(ip, "."))) < 4)
	return;
    
    call_out("timeout", TIMEOUT, id);
    
    for (ip="",i=sizeof(split); i--;)
	ip += split[i] + ".";
    ip += "in-addr.arpa";

    header = a16bit2str(id & 0xffff); /* msg ID */
    header += a16bit2str(0x100); 
    header += a16bit2str(0x1); /* qdcount */
    header += a16bit2str(0x0); /* ancount */
    header += a16bit2str(0x0); /* nscount */
    header += a16bit2str(0x0); /* arcount */

    qname = addr2qname(ip);
    qname += a16bit2str(0xc); /* type:  PTR */
    qname += a16bit2str(0x1); /* class: IN */

    debug("manwe", "resolve currently disabled\n");
    /*send_datagram(header + qname, DNS, PORT_DNS);*/
}

/*nomask void 
test(string ip, mixed ident)
{
  if (!ip)
    return;
    
  asker[++id] = ({ this_player(), "catch_tell", ident });

  _query(ip);
}*/

nomask void
query(string ip, string function, mixed ident)
{

  if (!ip)
    return;
    
  asker[++id] = ({ previous_object(), function, ident });

  _query(ip);
}

