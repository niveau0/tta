/*
 *   Telnet object for connecting other muds. Version 1.0/Muhveli
 *
 *
 *
 *  Future plans:
 *
 *  Use binary mode instead of telnet mode (NOT IN NEAR FUTURE)
 *
 *
 */ 

#include <mudcon.h>

string name;
object notify;
string buff;

varargs void do_finger(string address,string s, object o)
{
    object r;

    if(!s)
    {
	destruct_object();
	return;
    }

    name=s+"\n";
    notify=o;
    buff="";
    
    if(sscanf(address,"%*d.%*d.%*d.%*d")==4)
    {
	if(!s) return;
	// 79 for finger
	connect(address,79,"tcp");
    }
    else
    {
	r=DRIVER->query_resolver();
	if(!r)
	{
	    destruct_object();
	    return;
	}

	if(strlen(address)&&address[strlen(address)-1]!='.')
	    address+=".";
	
	r->getipnumber(address,"resolved");
	call_out("dns_timeout",30);
    }
}

void resolved(string a, string b)
{
    connect(a,79,"tcp");
}

void dns_timeout()
{
    notify->msg("[FINGER]: Dns timeout\n");
    destruct_object();
}

void receive_message(string str)
{
    buff+=str;
}

void open()
{
   send_message(name);
}

void close(int destr)
{  
   string str;
   if(buff!="") 
       notify->msg("[FINGER]:\n"+buff);
   else
       notify->msg("[FINGER]: Connect failed\n");   
   destruct_object();
}







