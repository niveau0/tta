/*
 * 4M code / written by Sieni@Muhveli
 * Intermud (aka 'mmm<->zombie link') protocol server and service daemon
 *
 */

/* Mud local configuration */
#include <mudcon.h>
/* Driver status() array defination */
#include <status.h>
/* Network definations */
#include <network.h>

int services;
mapping muds;
mapping addresses;
int opentime;
int debug;
int port;
mapping real_names;
mapping channels;
string thismud;

/* Setup the addresses */

void create()
{
    muds=([
	"MMM":
	/* IP_number     ,    port, delimiter1,       delimiter2            , protocol , level_as_creator_level, remote_creator_level */
	({ "192.89.182.3",    4000, "mmm-zombie-101", "kosminen-hubba+bubba", "MMMPROT", 201, 201 }),
	"Zombie": 
	({ "130.231.161.199", 4000, "mmm-zombie-101", "kosminen-hubba+bubba", "MMMPROT", 201, 90  }),
	"Delphine":
	({ "130.100.48.204",  4246, "mmm-zombie-101", "kosminen-hubba+bubba", "MMMPROT", 10 , 10  })
	]);
    
    addresses=([ 
	"192.89.182.3"  : "MMM",
	"130.231.161.199": "Zombie",
	"130.232.143.138": "Delphine",
	"194.100.48.204" : "Muhveli" 
	]);
    
    real_names=([
	"MMM"         : "Might, Magic and Mushrooms",
	"Zombie"      : "ZombieMUD",
	"Delphine"    : "Delphine MUD"
	]);
    
    /* Channel broadcast destinations */
    channels=([
	"net" : ({ "Zombie" }) 
	]);
    

}

/* Used by command 'connects' to tell the open time of port and port used */

int query_logintime() { return opentime; }
int query_port() { return port; }

/* Used by commands to check available muds */

string *query_muds() { return map_indices(muds); }

/* Send debug to Sieni */

void set_debug(int d)
{
     debug=d;
}

void send_sieni(string str)
{
    object obj;
    mapping windows;
    int mode;

    if(!debug) return;
      
    obj=find_user("sieni");
    mode=obj->query_mode();
   
    if(mode)
    {
	windows=obj->query_windows();
	if(!windows["3MRECV"]) obj->window(5,"3MRECV","UDP");
     
	obj->write_window("3MRECV",str);
    }
    else
	obj->msg("%(UDP) : "+str);
}

/* Called when port opens or closes */

void open()
{
    opentime=time();
}

void close() 
{
}

/* Process and send data to destination mud */

varargs static int send_data(string mud, mapping info, string extradata)
{
    string str;
    string *arr;
    int i;

    if(!mud || !muds[mud])
	return -1;
  
    info["mud_name"]=thismud;
  
    arr=map_indices(info);
  
    for(i=0;i<sizeof(arr);i++)
    {
	arr[i]=arr[i]+":"+info[arr[i]];    
    }
  
    str=implode(arr,muds[mud][2])+muds[mud][3]+extradata;

    send_sieni("(send) "+str+"\n");
    send_datagram(str, muds[mud][0], muds[mud][1]);      
    return strlen(str);
}

/* Send our status report on finger muhveli@localaddress */

static void muhveliinfo(mapping data, string mud)
{
   int i;
   string str;
   string str2;
   object ob;
   
   str="-----------------------------------------------------------------------\n";
   str2="Muhveli MUD Report - "+ctime(time());
   str=put_to_str(str,str2,strlen(str)/2-strlen(str2)/2);
   str+=" * Muhveli MUD is not currently open for public\n";
   str+=" * Current uptime "+ftime(status()[ST_UPTIME])+"\n";
   
   if(ob=find_object(SHUTD)) 
   {
      i=ob->query_shutdown();
      if(i)
        str+=" * Muhveli MUD will shutdown in "+ftime(i)+"\n";
   }
   
   str+=
       " * Net services available from Muhveli MUD to "+real_names[mud]+"\n"+
       "  - channel\n"+
       "  - finger information\n"+
       "  - tell service\n"+
       "  - who information\n"+
       "-----------------------------------------------------------------------\n";
   
   send_data(mud,
	     ([
		 "type"      :"inform",
		 "message"   :"Muhveli MUD report",
		 "user_name" :data["user_name"] ]),
	     str);
                    
    
}

/* Driver calls this when data arrives */

static int receive_data(string str, string ipnumber, int port)
{
    mapping info;
    string *arr;
    string mud;
    string data;
    int a,i;
    string id;
    object ob;
    mixed wanted;
  
    mud=addresses[ipnumber];
  
    send_sieni("(recv) "+mud+" "+port+" "+str+"\n");

    if(!mud)
	return 0;  /* Damn hackers, don't flood us with errors :) */

    if(muds[mud][4]=="MMMPROT") 
    {
	/* Protocol failure? */
	if(sscanf(str,"%s"+muds[mud][3]+"%s",str,data)!=2)
	    return 0;
    
	arr=explode(str,muds[mud][2]);
	info=([ ]);
  
	for(i=0;i<sizeof(arr);i++)
	{
	    if(sscanf(arr[i],"%s:%s",id,str)==2)
	    {
		info[id]=str;
	    }
	}
    
	switch(info["type"])
	{
	case "ping":
	    /* Foo! */
	    send_data(mud,([ "type" : "ping_answer" ]) );
	    break;
	case "channel":    
	    /* Pass the channel message to channel daemon */
	    if(member_array(mud,channels["net"])!=-1)
		CHANNELD->net_talk("net",mud,info,muds[mud]);
	    else
		log_file("nochan",
			 "["+ctime(time())[4..15]+"]"+
			 info["user_name"]+"@"+
			 mud+": "+info["message"]+"\n");
	    return 0;
	    break;
	case "inform":
	    /* Inform a user */
	    ob=find_user(info["user_name"]);
	    if(!ob)
		ob=find_player(info["user_name"]);

	    if(ob)
	    {
		if(!data||data=="0") data="";
		ob->start_more(explode("("+mud+": "+info["message"]+")\n"+
				       data,"\n"));
	    }      
	    break;
	case "tell":    
	    /* Process the tell here */

	    ob=find_user(info["target"]);

	    /* Su support */
	    if(!ob &&
	       (ob==find_player(info["target"])))
		ob=ob->query_user();
	    
	    if(!ob)
	    {
		send_data(mud,
			  ([ 
			      "type"      : "inform",
			      "message"   : "No such user online.",
			      "user_name" : info["user_name"] 
			      ]),
			  0);
		return 0;
	    }

	    /* Check ignore, add to last tells list, process for user */
		    
	    i=ob->receive_msg(lower_case(info["user_name"])+"@"
			      +lower_case(mud),
			      "tell",
			      capitalize(info["user_name"])+"@"+
			      info["mud_name"]+
			      " %^L_MAGENTA%^tells%^RESET%^ you: ",
			      info["message"]+"\n",0,0);
	    
	    
	    if(!i)
		id="Told to "+ob->query_name();
	    else
		id=ob->query_name()+" ignores you";
                          
	    send_data(mud,([ "type"      : "inform",
			   "message"   : id,
			   "user_name" : info["user_name"] ]),0);
	    break;
	case "who":
	    /* ARGH! Who wrote the interface for who command?!! */
   
	    wanted=
		no_colors(
		    implode("/cmds/all/_who"->do_who(0,1,1,1,1),"\n")
		    );

	    send_data(mud,
		      ([
			  "type"      : "inform",
			  "message"   : "Muhveli MUD who service reply",
			  "user_name" : info["user_name"] 
			  ]),
		      wanted);
	    break;
	case "finger":
	    if(info["target"]=="muhveli")
	    {
		muhveliinfo(info,mud);
		return 0;                    
	    }
	    
	    wanted=FINGERD->query_finger(info["target"]);
	    if(!stringp(wanted))
		send_data(mud,
			  ([ "type"      : "inform",
			   "message"   : "Muhveli MUD finger service reply",
			   "user_name" : info["user_name"] ]),
			  "No such user "+info["target"]+" in MuhveliMud\n");
	    else
		send_data(mud,
			  ([ "type"      : "inform",
			   "message"   : "Muhveli MUD finger service reply",
			   "user_name" : info["user_name"] ]),
			  no_colors(wanted));
	    break;                                                                                  
	default:
	    send_data(mud,
		      ([ 
			  "type"      : "inform",
			  "message"   : "Service "+info["type"]+
			  " is not available from Muhveli Mud.",
			  "user_name" : info["user_name"] 
			  ]),
		      0);
	    break;
	}
    }

    /* No other protocols implemented to this port */
      
    return 0;  
}

static int receive_datagram(string str, string ipnumber, int port)
{
    rlimits(20;2000000)  /* max stack depth 20, allow 2 million ticks used */
	{
	    receive_data(str,ipnumber,port);
	}
}

static int process_datagram()
{
    return 0;
}

/* Open the UDP port */
 
varargs void open_service(int p, string s)
{
    if(!p && !thismud && query_hostname()=="mmm.megabaud.fi")
    {
	p       = 4000;
	thismud = "MMM";
    }

    if(!p)
	port = PORT_UDPIMUD1;
    else
	port = p;
    
    if(!s)
	thismud = "Muhveli";
    else
	thismud = s;
    
    open_port("udp",port);
    return; 
}

/* Hooks for local commands */

void tell(object o,string target, string what, string mud)
{
    if(!mud || !what)
	return;

    send_data(mud,
	      ([
		  "type"      : "tell",
		  "user_name" : o->query_real_name(),
		  "user_level": (creatorp(o))? muds[mud][5]:1,
		  "message"   : what,
		  "target"    : lower_case(target)
		  ]),
	      0);
}

void finger(object o, string who, string mud)
{
    if(!who || !mud) 
	return;

    send_data(mud,
	      ([ 
		  "type"      : "finger",
		  "user_name" : o->query_real_name(),
		  "user_level": 0,
		  "target"    : lower_case(who) 
		  ]),
	      0);
}

void who(object o,string mud)
{
    send_data(mud,
	      ([
		  "type"      : "who",
		  "user_name" : o->query_real_name(),
		  "options"   : "" 
		  ]),
	      0);
}

void channel(string chan,string str,object who,int status)
{
    string *to;
    int i;

    to=channels[chan];
  
    str=no_colors(str);

    i=sizeof(to);

    while(i--)
    {
	send_data(to[i],
		  ([ 
		      "type"      : "channel",
		      "user_name" : who->query_real_name(),
		      "user_level": (creatorp(who))? muds[to[i]][5]:1,
		      "message"   : str
		      ]),
		  0);
    }
}

/* Sends test packages, don't abuse! */

int send_test(object o, string mud, int lenght)
{
   mapping info;
   string data;
   int i;
   
   info=([ 
       "type" : "test",
       "user_name" : o->query_real_name(),
       "user_level" : (creatorp(o))? muds[mud][5]:1 
       ]);
   
   data=new_string(lenght,'x');
   
   return send_data(mud, info, data);
}
