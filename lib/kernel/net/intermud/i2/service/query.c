#include "../i2config.h"

nomask string get_name(object ob)
{
    return ob->query_name();
}

nomask void
rcv_query(mapping data)
{
    int     i;
    string  tmp, *list;
    mapping ret, hosts;

    switch (data[DATA])
    {
      case "commands":
	  ret = I2_DEMON->query("hosts");
	  if (ret[lower_case(data[NAME])])
	      ret = ([
		  DATA: implode(ret[lower_case(data[NAME])][LOCAL_COMMANDS],
				":")
		  ]);
	  else
	      ret = ([ DATA: implode(I2_DEMON->query("commands"), ":") ]);
	  break;
      case "email":
	  ret = ([ DATA: ADMIN_EMAIL ]);
	  break;
      case "hosts":
	  tmp = "";
	  for(i=sizeof(list=map_indices(hosts=I2_DEMON->query("hosts"))); i--; )
	  {
	      tmp += hosts[list[i]][HOST_NAME] + ":" +
		     hosts[list[i]][HOST_IP] + ":" +
		     hosts[list[i]][HOST_UDP_PORT] + ":" +
		     implode(hosts[list[i]][LOCAL_COMMANDS], ",") + ":" +
		     implode(hosts[list[i]][HOST_COMMANDS], ",") + ":" +
		     hosts[list[i]][HOST_STATUS];
	      if (i)
		  tmp += "\n";
	  }
	  ret = ([ DATA: tmp ]);
	  break;
      case "inetd":
	  ret = ([ DATA: "0.7-tta" ]);
	  break;
      case "list":
	  /* List of things that can be queried. */
	  ret = ([ DATA: "commands\nemail\nhosts\ninetd\n" +
	                 "mud_port\ntime\nusers\nversion" ]);
	  break;
      case "mud_port":
	  ret = ([ DATA: PORT_TELNET ]);
	  break;
      case "time":
	  ret = ([ DATA: ctime(time()) ]);
	  break;
      case "users":
      {
	  object *u;
	  
	  tmp = "";
	  for (i=sizeof(u=users()); i--;)
	      tmp += u[i]->query_name() + "\n";
	  ret = ([ DATA: tmp ]);
	  break;
      }
      case "version":
	  ret = ([ DATA: MUDLIB_VERSION ]);
	  break;
      default:
	  /* Just ignore it for the time being. */
	  return;
    }
    I2_DEMON->send_udp(data[NAME], ret +
		       ([
			   REQUEST: REPLY,
			   RECIPIENT: data[SENDER],
			   ID: data[ID],
			   QUERY: data[DATA]
			   ]) );
}
