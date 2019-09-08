#include <mudcon.h>

#include <net/ftpd.h>
#include <net/ftpdconf.h>
#include <net/ftpdsupp.h>

object connect;
string data;
int port;
string type;
int status;
int pos;
int len;
string data_mode_name;
string name;
string addr;
string mode;
int append;
string dir;

static void send_data() 
{
    int length, ret_val;
    string tmp;
    int tmppos;

    if ( dir == "DOWNLOAD" )
        return;
    
    length = ( (pos+BLOCK_SIZE) >= len ) ?
	     ( len - pos ) : BLOCK_SIZE;

    if ( type == "ASCII" )
    {
    	tmppos = pos;
        pos += len;
	while ( !send_message( data[tmppos..(tmppos+length-1)]) )
	{
            length = ( (pos+BLOCK_SIZE) >= len ) ?
	             ( len - pos ) : BLOCK_SIZE;	    
	    tmppos=pos;
	    pos += length;
	    
	    if ( pos >= len )
	    {
	        if(!send_message(-1))
	        {
		    connect->dataconn_tcompelete();
		    destruct_object();
		    return;	        
	        }	    
		return;
	    }
	    

	}
	
	if ( pos >= len )
	{
	    if(!send_message(-1))
	    {
		connect->dataconn_tcompelete();
		destruct_object();
		return;	        
	    }	    
	}
    } 
    else
    {
        tmp=read_file(data,pos,length);
	pos += length;
	    
	while ( !send_message(tmp) )
	{
            length = ( (pos+BLOCK_SIZE) >= len ) ?
	             ( len - pos ) : BLOCK_SIZE;	    

	    if ( pos >= len )
	    {
	        if(!send_message(-1))
	        {
		    connect->dataconn_tcompelete();
		    destruct_object();
		    return;
	        }
		return;
	    }
            
            tmp=read_file(data,pos,length);
            pos += length;
	}
	
	if ( pos >= len )
	{
	    if(!send_message(-1))
	    {
		connect->dataconn_tcompelete();
		destruct_object();
		return;
	    }
	}
	        
    }
} /* data_write_callback() */

static void receive_message(string str)
{
    if(dir=="UPLOAD")
        return;

    if(data_mode_name=="ASCII")
        str=implode(explode(str,"\r"),"");     
    
    if(append!=1&&!pos)
    {
        pos=1;
        if(file_exists(data))
        {
            rm(data);
        }
    }
        
    write_file(data,str);
        
}

static void open()
{
    status=1;
    update_rights();
    
    if(type!="ASCII")
       len=file_size(data); 
    if(dir=="UPLOAD")
    {
        connect->dataconn_uopened(len,name,data_mode_name);
        send_data();
    }
    else 
    if(dir=="DOWNLOAD")
        connect->dataconn_dopened(name,data_mode_name);
         
}

static void close(int destr) 
{     
    if(!status)
    {
        if(connect)
            connect->dataconn_fail();
        if(!destr) destruct_object();
        return;
    }
    else if(dir=="DOWNLOAD")
    {
        if(connect)
            connect->dataconn_tcompelete();
        if(!destr) destruct_object();
    }
    
}

void data_conn(object master,
               string mess,
               string n,
               string t, 
               string m,
               int    p,
               string a )
{
    int ret;
    string *arr;
    int i,sz;
    mixed tmp;

    if(base_name(previous_object())!="/system/net/ftpcontrol"||
       !query_ip_number(previous_object()))
        error("somebody is hacking around");

    
    type=t;
    connect=master;
    name=n;
    port=p;
    addr=a;
    mode=m;

    if ( type == "ASCII" ||
	  ( type == "FILE" && mode == "ASCII" ) )
    {
	data_mode_name = "ASCII";
    } 
    else 
    {
	data_mode_name = "BINARY";
    }

    connect(addr,port,"tcp");
    

    if(type=="ASCII")
    {
        data=implode(explode("\n"+mess+"\n","\n"),"\r\n");
        len=strlen(data);
    }
    else
    {
        data=mess;
    }
        
    pos=0;
        
    dir="UPLOAD";    
    
}  /* data_conn() */


void message_done()
{
    if ( pos >= len )
    {
        connect->dataconn_tcompelete();
        
        destruct_object();
        return;
    }
    else
        send_data();
}

void read_connection(object master,
                     string fle,
                     int    app,
                     string m,
                     int    p,
                     string a )
{
    int ret;
    string *arr;
    int i,sz;

    mixed tmp;
    
    if(base_name(previous_object())!="/system/net/ftpcontrol"||
       !query_ip_number(previous_object()))
        error("somebody is hacking around");
    
    connect=master;
    data=fle;
    append=app;
    
    port=p;
    addr=a;
    mode=m;
    
    if( mode == "ASCII" )
    {
	data_mode_name = "ASCII";
    } 
    else 
    {
	data_mode_name = "BINARY";
    }
    
    connect(addr,port,"tcp");
    
    dir="DOWNLOAD";
    
    pos=0;
    
}  /* data_conn() */

object ftp_control()
{
    return connect;
}