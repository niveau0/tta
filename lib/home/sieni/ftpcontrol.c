/*
 *  Originally from TMI-2 1.2
 *
 */ 

#include <mudcon.h>
#include <status.h>

#include <net/ftpd.h>
#include <net/ftpdconf.h>
#include <net/ftpdsupp.h>

#define FTPD_VERSION "5.8"
#define FTP_DATACONN "/system/net/ftpdata"

string name;
int    sec_level;
string status;
int    timestamp;
string conntype;
int    logged;
string site;
string current_dir;
string userid;

int    session_id;
mixed  *data_for_id;

int    data_port;
string data_addr;
string data_type;
object data_connect;

string from_name;

mapping cmdtab;
mapping sitecmdtab;

int     opentime;

void create()
{
    cmdtab = ([
    "user" : ({ "USER", 1, "<sp> username" }),
    "pass" : ({ "PASS", 1, "<sp> password" }),
    "acct" : ({ "ACCT", 0, "(specify account)" }),
    "smnt" : ({ "SMNT", 0, "(structure mount)" }),
    "rein" : ({ "REIN", 0, "(reinitialize server state)" }),
    "quit" : ({ "QUIT", 1, "(terminate service)", }),
    "port" : ({ "PORT", 1, "<sp> b0, b1, b2, b3, b4" }),
    "pasv" : ({ "PASV", 0, "(set server in passive mode)" }),
    "type" : ({ "TYPE", 1, "<sp> [ A | E | I | L ]" }),
    "stru" : ({ "STRU", 0, "(specify file structure)" }),
    "mode" : ({ "MODE", 0, "(specify transfer mode)" }),
    "retr" : ({ "RETR", 1, "<sp> file-name" }),
    "stor" : ({ "STOR", 1, "<sp> file-name" }),
    "appe" : ({ "APPE", 1, "<sp> file-name" }),
    "mlfl" : ({ "MLFL", 0, "(mail file)" }),
    "mail" : ({ "MAIL", 0, "(mail to user)" }),
    "msnd" : ({ "MSND", 0, "(mail send to terminal)" }),
    "msom" : ({ "MSOM", 0, "(mail send to terminal or mailbox)" }),
    "msam" : ({ "MSAM", 0, "(mail send to terminal and mailbox)" }),
    "mrsq" : ({ "MRSQ", 0, "(mail recipient scheme question)" }),
    "mrcp" : ({ "MRCP", 0, "(mail recipient)" }),
    "allo" : ({ "ALLO", 1, "allocate storage (vacuously)" }),
    "rest" : ({ "REST", 0, "(restart command)" }),
    "rnfr" : ({ "RNFR", 1, "<sp> file-name" }),
    "rnto" : ({ "RNTO", 1, "<sp> file-name" }),
    "abor" : ({ "ABOR", 1, "(abort operation)" }),
    "dele" : ({ "DELE", 1, "<sp> file-name" }),
    "cwd"  : ({ "CWD" , 1, "[ <sp> directory-name ]" }),
    "xcwd" : ({ "XCWD", 1, "[ <sp> directory-name ]" }),
    "list" : ({ "LIST", 1, "[ <sp> path-name ]" }),
    "nlst" : ({ "NLST", 1, "[ <sp> path-name ]" }),
    "site" : ({ "SITE", 1, "site-cmd [ <sp> arguments ]" }),
    "syst" : ({ "SYST", 1, "(get type of operating system)" }),
    "stat" : ({ "STAT", 1, "[ <sp> path-name ]" }),
    "help" : ({ "HELP", 1, "[ <sp> <string> ]" }),
    "noop" : ({ "NOOP", 1, "" }),
    "mkd"  : ({ "MKD" , 1, "<sp> path-name" }),
    "xmkd" : ({ "XMKD", 1, "<sp> path-name" }),
    "rmd"  : ({ "RMD" , 1, "<sp> path-name" }),
    "xrmd" : ({ "XRMD", 1, "<sp> path-name" }),
    "pwd"  : ({ "PWD" , 1, "(return current directory)" }),
    "xpwd" : ({ "XPWD", 1, "(return current directory)" }),
    "cdup" : ({ "CDUP", 1, "(change to parent directory)" }),
    "xcup" : ({ "XCUP", 1, "(change to parent directory)" }),
    "stou" : ({ "STOU", 1, "<sp> file-name" }),
    "size" : ({ "SIZE", 1, "<sp> path-name" }),
    "mdtm" : ({ "MDTM", 1, "<sp> path-name" })
    ]);
 
    sitecmdtab = ([
    "idle"  : ({ "USER",  1, "[ <sp> timeout ]" }),
    "umask" : ({ "UMASK", 0, "[ <sp> umask ]" }),
    "chmod" : ({ "CHMOD", 0, "<sp> path-name" }),
    "time"  : ({ "TIME",  1, "(return local time)" }),
    "help"  : ({ "HELP",  1, "[ <sp> <string> ]" })
    ]);

}

string query_address()
{
   int i;
   
   if(!sscanf(userid,"%d",i))
       return userid+"@"+site;         
   else
       return site+":"+userid;
}

string query_name()
{
    if(logged)
	return name;
}

int query_logintime()
{
    return opentime;
}

void set_dataid(int id)
{
    session_id=id;
}

void set_userid(string uid)
{
    userid=uid;
}

void set_hostname(string host)
{
    site=host;
}

void lose_connection()
{
    
}

void lose_user(int i)
{
    if(!i) destruct_object();
}

/*
void setup_ftp( int port );
void finish_lookup( string host, string number );
void resolve_callback( string address, string resolved, int key );
string get_path( int fd, string str );
int strncmp(string str, string sub, int i);

int strncmp(string str, string sub, int i) 
{
    int j;

    j = strlen(str);
    return (i > j ? strcmp(str, sub) : strcmp(str[0..i-1], sub));
}
*/

int check_valid_read( string fname ) 
{
    string file;
    file=test_file_access(fname, 1, query_accesses() );

    if(!file) return 0;
    
    return 1;
}

int check_valid_write( string fname ) 
{
    string file;
    file=test_file_access(fname, 2, query_accesses() );
    
    if(!file) return 0;
    
    return 1;
}

void open()
{
    string address;
    int res;

    opentime=time();

    res=FTPPORT->ok_to_connect(session_id);
    
    if(res==2)
    {
        send_message("530 Too many ftp connections active.  Try again later.\n");
        destruct_object();
    }
    else if(res)
    {
        send_message("530 Error occured, closing down.\n");
        destruct_object();
    }

    send_message("220- Muhveli FTP server (Version "+FTPD_VERSION+" [DGD/LPC]) ready.\n"+
   	         "220  Please login as yourself.\n");
}

string ls( string path, int column ) 
{
    string *files;
    int b, i, j, s;
    mixed *stats;
    string tmp, tmp2, creator, domain;
    mixed *dir_data;
    string str;
    
    string *out;
     
    path=reduce_path(path,current_dir,name);
            
    i = strlen(path);

    if(file_size(path)==-2)
    {    
        if ( path[i - 1] == '/') 
        {
            path += "*";
        }    
        else
        {
            path += "/*";
        } 
    }

    dir_data=get_dir(path);

    if(!sizeof(dir_data))
        return "";

    if (column)
    {       
	files = dir_data[0];;

	/* can only happen if permissions are messed up at account level */
	if (!files)
	    return "";

	files -= ({ ".", ".." });

	if (!(i = sizeof( files )))
	    return "";

	while ( i-- ) 
	{
	    if ( dir_data[1][i] == -2 )
	    {
		files[i] += "/";
	    }
	}
	return implode( files, "\n" )+"\n";
    }

    // the Unix-like file permissions are mainly for effect...hopefully it
    // isn't too much, since anything more would likely be too cpu intensive
    // and cause it to max eval...
//    creator = (string)MASTER_OB->creator_file(path);
//    if (!creator)  creator = ROOT_UID;

//    domain = (string)MASTER_OB->domain_file(path);
//    if (!domain)  domain = ROOT_UID;

/*    i = strsrch(path, '/', -1);
    if (i >= 0)
	path = path[0..i]; */

    out=({ });

    s=sizeof(dir_data[0]);
  
    for (i = 0; i < s; i++)
    {
	/* process timestamp */
	tmp2 = ctime(dir_data[2][i]); /* get last modified timestamp */
	
	if (dir_data[2][i] + SECS_IN_YEAR < time())
	{
	    /* MMM DD  YYYY */
	    tmp = tmp2[4..9]+" "+tmp2[20..23];
	} else
	{
	    /* MMM DD hh:mm */
	    tmp = tmp2[4..15];
	}

	j = dir_data[1][i];   /* get filesize */
	if (j == -2) 
	{
	    /* directory */
	    j=strlen(dir_data[0][i]);
	    
	    str = "drwxrwsr-x mud        mud             <DIR>  "+
	          tmp+" "+
	          dir_data[0][i];
            out+=({ str });
	          
	} 
	else 
	{
	    b=strlen(j+"");
	    
	    str = "-rwxrwsr-x mud        mud       "+
	          "          "[..10-b]+j+"  "+
	          tmp+" "+
	          dir_data[0][i];
	    out+=({ str });
	}
    }

    return implode( out, "\n" )+"\n";
}

void dataconn_fail()
{
    if(previous_object()!=data_connect) error("Sqruiik?");
    
    send_message( "425 Can't build data connection.\n" );    
}

void dataconn_uopened(int len, string name, string type)
{
    if(previous_object()!=data_connect) error("Sqruiik?");
    send_message("150 Opening "+type+" mode data connection for "+name+" "+
    	         "("+len+" bytes).\n");
}

void dataconn_dopened( string name, string type)
{
    if(previous_object()!=data_connect) error("Sqruiik?");
    send_message("150 Opening "+type+" mode data connection for "+name+".\n");
}

void dataconn_tcompelete()
{
    if(previous_object()!=data_connect) error("Sqruiik?");
    send_message("226 Transfer compelete.\n");
}

void dataconn_ferror()
{
    if(previous_object()!=data_connect) error("Sqruiik?");
    send_message("551 Error on input file.\n");
}

static void data_conn( string mess, string name, string type )
{
    if ( data_connect )
    {
	send_message( "425 Can't open data connection.\n" );
	return;
    }

    data_connect=clone_object(FTP_DATACONN);
    data_connect->data_conn(this_object(),
                            mess,
                            name,
                            type,
                            data_type,
                            data_port,
                            data_addr);
}  /* data_conn() */

static void read_connection( string path, int append )
{
    if ( data_connect )
    {
	send_message( "425 Can't open data connection.\n" );
	return;
    }

    data_connect=clone_object(FTP_DATACONN);
    data_connect->read_connection(this_object(),
                                  path,
                                  append,
                                  data_type,
                                  data_port,
                                  data_addr);
}  /* read_connection() */

static void logout()
{
    logged = 0;
    if ( name )
    {
#ifdef LOG_TIME
	log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_CONNECT
	log_file( LOG_FILE, name+" logged out from "+site+":"+userid+".\n");
#endif

    }
}

/*
 * parse user command
 */
static void parse_comm( string str ) 
{
    string *command, tmp, tmp2;
    mixed *misc;
    int port, i,j , s;
    string poro;

    command = explode( str, " " );
    
    timestamp = time();
    
    switch( lower_case( command[ 0 ] ) )
    {
	case "port": // specify data connection port
	    command = explode( implode( command[ 1.. ], " " ), "," );
	    
	    if ( sizeof( command ) < 6 )
		send_message( "550 Failed command.\n" );
	    else
	    {
		data_addr = implode( command[ 0..3 ], "." );
		
		sscanf( command[ 4 ], "%d", i );
		port = i << 8;
		sscanf( command[5],"%d",i );
		port += i;
		
		data_port = port;
		send_message( "200 PORT command successful.\n" );
	    }
	    break;
	case "user": // specify user name
	    if ( logged )
	    {
		logout();
		name = 0;
	    }

	    CHECK_CMD(1);

	    if ( SECURITYD->query_seclevel(command[1]) < 5 ) 
	    {
		send_message("530 User "+command[1]+" access denied.\n");
#ifdef LOG_TIME
		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_NO_CONNECT
		log_file( LOG_FILE, command[1]+" denied access from "+site+".\n");
#endif
	    } 
	    else 
	    {
		send_message("331 Password required for "+ command[ 1 ] +"\n" );
 		name = command[ 1 ];
	    }
	    break;
	case "pass": // specify password
	    if ( logged || !name )
	    {
		send_message( "503 Log in with USER first.\n" );
		break;
	    }

	    if ( !check_password( name,
		  implode( command[ 1.. ], " " ) ) )
	    {
         	send_message( "530 Login incorrect.\n" );

#ifdef LOG_TIME
		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_NO_CONNECT
		log_file( LOG_FILE, 
		          name+" failed/incorrect login from "+site+".\n" );
#endif
		name = 0;
	    } 
	    else
	    
#ifdef CHECK_SITE
//	    if (!check_site( UNAME, fd )) {
//		/*
//		 * Note: this particular response of 530 is not mentioned
//		 * as a possible response to the PASS command in RFC959,
//		 * because site checking is TMI specific.
//		 */
//		socket_write( fd, sprintf("530 User %s: Can't login from %s.\n",
//		      UNAME, USITE) );
#ifdef LOG_TIME
//		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_NO_CONNECT
//		log_file( LOG_FILE, sprintf("%s refused login from %s.\n",
//		      UNAME, USITE) );
#endif
//		UNAME = 0;
//		lose_user( fd );
//	    } else
#endif
	    {
		logged = 1;
		sec_level=SECURITYD->query_seclevel(name);
		update_rights();
                current_dir = "/players/"+name+"/";
#ifdef LOG_TIME
		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_CONNECT
		log_file( LOG_FILE, name+" connected from "+site+":"+userid+".\n");
#endif

		if ( file_size( current_dir )!= -2 )
		{
		    send_message( "230 No directory! Logging in with home="+
			          "/\n" );
		    current_dir = "/";
		}
		else 
		{
#ifdef LOGIN_MSG
		    if(file_exists(LOGIN_MSG))
		    {
 			tmp = read_file( LOGIN_MSG );
			tmp = "230- "+
			      implode( explode(tmp, "\n"), "\n230- " )+"\n";
			send_message(tmp);
		    }
#endif
		    send_message( "230 User "+name+" logged in successfully.\n" );
		}
		
                data_type="BINARY";
                status="command";
	    }
	    break;
	case "allo": // allocate storage (vacuously)
	    send_message( "202 ALLO command ignored.\n" );
	    break;
	case "noop": // do nothing
	    send_message( "200 NOOP command successful.\n" );
	    break;
	case "retr": // retrieve a file
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    
	    tmp = reduce_path( command[ 1 ], current_dir,name );	    
	    
	    if ( !check_valid_read( tmp ) )
	    {
		PERMISSION_DENIED550(tmp);
	    }
	    else
	    if ((i = file_size(tmp)) == -2)
	    {
		send_message("550 "+command[1]+": Not a plain file.\n");
	    } 
	    else if (i == -1) 
	    {
		send_message("550 "+command[1]+": No such file or directory.\n");
	    } 
	    else 
	    {
#ifdef LOG_TIME
		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
		log_file( LOG_FILE, name+" retr "+tmp+" ("+data_type+").\n");
#endif
		data_conn( tmp, command[ 1 ], "FILE" );
	    }
	    break;
	case "stou": // store a file with a unique name
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[ 1 ], current_dir, name );
	    i = file_size(tmp);
	    if (i >= 0 && check_valid_write( tmp ))
	    {
		for (i = MAX_UNIQUE_TRIES; i; i--)
		{
		    if( file_size(tmp+i) == -1)
			break;
		}
		if (i)
		{
		    tmp = tmp+i;
#ifdef FILE_LOCKING
		    if (flock_wrapper(tmp, F_LOCK, fd))
		    {   /* parent fd */
#endif

#ifdef LOG_TIME
			log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
			log_file( LOG_FILE, name+" stou "+tmp+" ("+data_type+").\n" );
#endif
			read_connection( tmp, -1 );
#ifdef FILE_LOCKING
		    }
		    else
		    {
			PERMISSION_DENIED550(tmp);
		    }
#endif
		}
		else
		{
		    message( "452 Unique file name cannot be created.\n");
		}
	    }
	    else
	    {
		PERMISSION_DENIED553(tmp);
	    }
	    break;
	case "stor": // store a file
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[ 1 ], current_dir, name );
	    if ( file_size(tmp) != -2 && check_valid_write( tmp ) )
	    {
#ifdef FILE_LOCKING
		if (flock_wrapper(tmp, F_LOCK, fd))
		{   /* parent fd */
#endif

#ifdef LOG_TIME
		    log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
		    log_file( LOG_FILE, name+" stor "+tmp+" ("+data_type+").\n");
#endif
		    read_connection( tmp, 0 );
#ifdef FILE_LOCKING
		}
		else
		{
		    PERMISSION_DENIED550(tmp);
		}
#endif
	    }
	     else 
	    {
		PERMISSION_DENIED553(tmp);
	    }
	    break;
	case "appe": // append to a file
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path(command[1], current_dir, name);

	    if ( file_size(tmp) >= 0 && check_valid_write( tmp ) )
	    {
#ifdef FILE_LOCKING
		if (flock_wrapper(tmp, F_LOCK, fd)) {   /* parent fd */
#endif

#ifdef LOG_TIME
		    log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
		    log_file( LOG_FILE, name+" appe "+tmp+" ("+data_type+").\n" );
#endif
		    read_connection( tmp, 1 );
#ifdef FILE_LOCKING
		}
		else
		{
		    PERMISSION_DENIED550(tmp);
		}
#endif
	    }
	    else
	    {
		PERMISSION_DENIED553(tmp);
	    }
	    break;
	case "dele": // delete a file
	    CHECK_LOGIN();
	    CHECK_CMD(1);	    
	    tmp = reduce_path( command[1], current_dir, name );
	    if ((i = file_size(tmp)) == -2)
	    {
		send_message( "550 "+command[1]+": Not a plain file.\n");
	    }
	    else if (i == -1)
	    {
	        send_message( "550 "+command[1]+": No such file or directory.\n");
	    } 
	    else if ( check_valid_write( tmp ) )
	    {
#ifdef FILE_LOCKING
		if (flock_wrapper(tmp, F_LOCK, fd)) {   /* parent fd */
#endif

#ifdef LOG_TIME
		    log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
		    log_file( LOG_FILE, name+" dele "+tmp+".\n");
#endif
		    catch( i = rm(tmp) );
		    if (i)
			send_message( "250 DELE command successful.\n" );
		    else
			send_message( "550 Permission denied to "+tmp+".\n" );
#ifdef FILE_LOCKING
		    flock_wrapper(tmp, F_UNLOCK, fd);   /* parent fd */
		} 
		else
		{
		    PERMISSION_DENIED550(tmp);
		}
#endif
	    } else {
		PERMISSION_DENIED553(tmp);
	    }
	    break;
	/* Supposed to return modified time in the format: YYYYMMDDHHMMSS */
	case "mdtm": // show last modification time of file
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path(command[ 1 ], current_dir, name);
	    
	    if ( check_valid_read( tmp ) ) {
		if ((i = file_size(tmp)) == -2)
		    send_message( "550 "+command[1]+": Not a plain file.\n");
		else if (i == -1)
		    send_message( "550 "+command[1]+": No such file or directory.\n");
		else
		{
		    tmp2 = ctime(file_date(tmp));
		    j=member_array(tmp2[4..6], MONTHS)+1;
		    if(strlen(j+"")<2) poro="0";
		    else poro="";
		    
		    tmp2 = tmp2[20..23]+
		           poro+j+
 			   ((tmp2[8] == ' ') ? "0" : tmp2[8..8])+
 			   tmp2[9..9]+
 			   tmp2[11..12]+tmp2[14..15]+tmp2[17..18];
 			   
#ifdef LOG_TIME
		    log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_CD_SIZE
		    log_file( LOG_FILE, name+" mdtm "+tmp+".\n" );
#endif
		    send_message( "213 "+tmp2+"\n" );
		}
	    } else
		PERMISSION_DENIED550(command[ 1 ]);
	    break;
	case "size": // return size of file
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[ 1 ], current_dir, name);
	    
	    if ( check_valid_read( tmp ) )
	    {
		if ( file_size( tmp )==-2 )
		    send_message( "550 "+command[1]+": Not a plain file.\n" );
		else
		    if ( !file_exists( tmp ) )
			send_message( "550 "+command[1]+" does not exist.\n" );
		    else {
#ifdef LOG_TIME
			log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_CD_SIZE
			log_file( LOG_FILE, name+" size "+tmp+".\n");
#endif
			send_message( "215 "+ file_size( tmp )+"\n" );
		    }
	    } else
		PERMISSION_DENIED550(command[ 1 ]);
	    break;
	case "nlst": // give name list of files in directory
	    CHECK_LOGIN();
	    /* Send name list */
	    if ((i = sizeof(command)) > 1 && command[1] == "-l")
	    {
		if (i == 2)
		    command[1] = ".";
		else
		    command = ({ command[0] }) + command[2..];
		// and fall through to "list"
	    } 
	    else 
	    {
		/* Used by commands like "dir", "mget", and "mput" */
		if ( i > 1 )
		    tmp = reduce_path( command[ 1 ], current_dir, name );
		else
		    tmp = current_dir;

		if ( check_valid_read( tmp ) )
		{
		    tmp2 = ls( tmp, 1 );
		    if (tmp2 == "")
			send_message( "550 No files found.\n" );
		    else
			data_conn( tmp2, "ls", "ASCII" );
		} else
		    PERMISSION_DENIED550(tmp);
		break;
	    }
	case "list": // give list files in a directory
	    CHECK_LOGIN();
	    /* Send directory file list (like exec'ing "ls") */
	    if ( sizeof( command ) > 1 )
		tmp = reduce_path(command[1], current_dir, name);
	    else
		tmp = current_dir;
		
	    if ( check_valid_read( tmp ) )
	    {
		tmp2 = ls( tmp, 0 );
		if (tmp2 == "")
		    send_message( "550 No files found.\n");
		else
		    data_conn( tmp2, "ls", "ASCII" );
	    } 
	    else
		PERMISSION_DENIED550(tmp);
	    break;
	case "xpwd": // print the current working directory (deprecated)
	case "pwd":  // print the current working directory
	    CHECK_LOGIN();
	    send_message("257 \""+current_dir+"\" is the current directory.\n");
	    break;
	case "xcup": // change to parent of current working directory (deprecated)
	case "cdup": // change to parent of current working directory
	    if (sizeof(command) > 1)
		command[ 1 ] = "..";
	    else
		command += ({ ".." });
	case "xcwd": // change working directory (deprecated)
	case "cwd":  // change working directory
	    CHECK_LOGIN();
	    if ( sizeof( command ) > 1 )
		tmp = reduce_path((command[ 1 ]=="/")?"/":(command[1] +"/"), current_dir, name);
	    else
		tmp = current_dir;
		
	    if ( check_valid_read( tmp ) )
	    {
		if ( file_size( tmp ) != -2 )
		{
		    send_message("550 "+tmp+": Not a directory.\n" );
		    break;
		}
#ifdef LOG_TIME
		log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_CD_SIZE
		log_file( LOG_FILE, name+" cd to "+tmp+".\n" );
#endif
		current_dir = tmp;
/*
#ifdef MESSAGE_FILES
		if (file_size(tmp + "/.message") > 0) {
		    tmp = read_file(tmp + "/.message");
		    tmp = "250- "+
		          implode(explode(tmp,"\n"),"\n250- ")+
		          "\n250 "+command[0]+" command successful.\n";
		    send_message(tmp);
		}
		else
#endif
*/
		send_message( "250 "+command[0]+" command successful.\n");
	    } else
		send_message( "550 "+tmp+": No such file or directory.\n");
	    break;
	case "quit": // terminate session
	    send_message( "221 Goodbye.\n" );
	    logout();
	    lose_user(0);
	    break;
	case "type": // specify data transfer type
	    CHECK_CMD(1);
	    if ( command[ 1 ] == "I" ) {
		data_type = "BINARY";
		send_message( "200 Type set to I.\n" );
	    } else
		if ( command[ 1 ] == "A" ) 
		{
		    data_type = "ASCII";
		    send_message( "200 Type set to A.\n" );
		} 
		else
		    send_message( "504 Type "+command[1]+" not implemented.\n");
	    break;
	case "stat": // return status of server
	    send_message( "211 FTP server status: Muhveli MUD FTP Version "+
	                  FTPD_VERSION+"\r\n"+
	                  "     Connected to "+site+"\r\n");
	    if ( logged )
		send_message("    Logged in as "+name+"\r\n" );
	    else
		if ( name )
		    send_message( "    Waiting for password.\r\n" );
		else
		    send_message( "    Waiting for user name.\r\n" );
		    
	    send_message( "    TYPE: ASCII, FORM: Nonprint; STRUcture: "+
			  "File; transfer MODE: Stream.\r\n" );
			  
	    if ( data_connect )
		send_message( "    Data connection open.\r\n" );
	    else
		if ( data_addr )
		    send_message( "    "+data_addr+" "+data_port+"\r\n");
		else
		    send_message( "    No data connection.\r\n" );
		    
	    send_message( "211 End of status.\n" );
	    break;
	case "abor": // abort previous command
	    /* Stops recvs and stors. At least thats what it is supposed to do. */
	    if ( data_connect )
	    {
		send_message( "426 Transfer aborted. Data connection closed.\n");
#ifdef FILE_LOCKING
		if ( (socket_info[ fd ][ DATA_FD ])[ TYPE ] == DOWNLOAD )
		    flock_wrapper( (socket_info[ fd ][ DATA_FD ])[ PATH ],
			  F_UNLOCK, socket_info[ fd ] );
#endif
                destruct_object();
	    }
	    send_message( "226 Abort successful.\n" );
	    break;
	case "syst": // show operating system type of server system
	    send_message( "215 Muhveli MUD running under DGD.\n" );
	    break;
	case "xmkd": // make a directory (deprecated)
	case "mkd":  // make a directory
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[ 1 ],current_dir,name );
	    if (file_size( tmp ) == -1)
	    {
		if (check_valid_write(tmp ))
		{
		    if (mkdir(tmp))
		    {
#ifdef LOG_TIME
			log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
			log_file( LOG_FILE, " mkd "+tmp+".\n" );
#endif
			send_message("257 "+command[0]+" command successful.\n");
		    } else
			send_message("550 "+command[0]+" command failed.\n");
		} else
		    PERMISSION_DENIED550(command[ 1 ]);
	    } 
	    else
		send_message("550 "+command[1]+": Directory or file already exists.\n");
	    break;
	case "xrmd": // remove a directory (deprecated)
	case "rmd":  // remove a directory
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[ 1 ], current_dir, name );
	    if (file_size( tmp ) == -2)
	    {
		if (check_valid_write(tmp))
		{
		    if (rmdir(tmp))
		    {
#ifdef LOG_TIME
			log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
			log_file( LOG_FILE, name+" rmd "+tmp+".\n");
#endif
			send_message("250 "+command[0]+" command successful.\n");
		    } 
		    else
			send_message("550 "+command[0]+" command failed.\n");
		} 
		else
		    PERMISSION_DENIED550(command[ 1 ]);
	    } 
	    else
		send_message( "550 "+command[1]+": No such file or directory.\n");
	    break;
	case "site": // non-standard commands
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    switch (lower_case(command[ 1 ]))
	    {
/*		case "idle":
		    if (sizeof(command) < 3) {
			send_message("200 Current IDLE time limit is "+" seconds; max %d\n",
			      socket_info[fd][IDLE_SETTING], MAX_FTPD_IDLE));
			break;
		    }

		    if (!sscanf(command[2], "%d", i)) {
			socket_write(fd, "550 SITE IDLE command failed.\n");
			break;
		    }

		    i = (i < FTPD_TIMEOUT ? FTPD_TIMEOUT : (i > MAX_FTPD_IDLE ? MAX_FTPD_IDLE : i));
		    socket_info[fd][IDLE_SETTING] = i;
		    socket_write(fd, sprintf("200 Maximum IDLE time set to %d seconds\n", i));
		    break;
		case "time":
		    socket_write(fd,
			  sprintf("200 Local TIME is %s.\n", ctime(time())[4..15] ) );
		    break;
		case "help":
		    if (sizeof(command) > 1) {
			tmp = lower_case(command[1]);
			if (!undefinedp(sitecmdtab[ tmp ])) {
			    misc = sitecmdtab[ tmp ];
			    if (misc[1])
				socket_write(fd, sprintf("214 Syntax: %s %s.\n",
				      misc[0], misc[2]) );
			    else
				socket_write(fd, sprintf("214 %s %s; unimplemented.\n",
				      misc[0], misc[2]) );
			} else {
			    socket_write(fd, sprintf("502 Unknown command %s.\n",
				  command[ 1 ]) );
			}
		    } else {
			socket_write(fd, "214- The following SITE commands are recognized "
			      "(* =>'s unimplemented).\n214-\n");
			misc = keys(sitecmdtab);
			s = sizeof(misc);
			tmp = "214- ";
			for (i = 0; i < s; i++) {
			    tmp += sprintf("%c%-7s", sitecmdtab[misc[i]][1] ? ' ' : '*',
				       sitecmdtab[misc[i]][0]);
			    if (i % 8 == 7) {
				socket_write(fd, tmp + "\n");
				tmp = "214- ";
			    }
			}
			if (i % 8)
			    socket_write(fd, tmp + "\n");
			socket_write(fd, sprintf("214-\n214 Direct comments to %s.\n",
			      FTP_BUGS_EMAIL));
		    }
		    break;
		case "chmod":
		case "umask":
		    socket_write( fd, sprintf("502 '%s' command not implemented.\n",
			  command[ 0 ]) );
		    break; */
		default:
		    send_message( "500 '"+command[0]+" "+command[1]+" ' command "+
			  "not understood.\n");
		    break;
	    }
	    break; 
	case "rnfr": // specify rename-from file name
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    tmp = reduce_path( command[1], current_dir, name );
	    
	    i = file_size( tmp );
	    
	    if (i == -2)
	    {
		send_message( "550 "+command[1]+": Not a plain file.\n");
	    } 
	    else if (i == -1)
	    {
		send_message( "550 "+command[1]+": No such file or directory.\n");
	    }
	    else if (check_valid_write(tmp))
	    {
		from_name = tmp;
		send_message( "350 File exists, ready for destination name.\n");
	    }
	    else 
		PERMISSION_DENIED550(command[ 1 ]);
	    break;
	case "rnto": // specify rename-to file name
	    CHECK_LOGIN();
	    CHECK_CMD(1);
	    if (from_name)
	    {
		send_message( "503 Bad sequence of commands.\n");
	    } 
	    else
	    {
		tmp = reduce_path( command[1], current_dir, name);
		i = file_size( tmp );
		if (i != -1)
		{
		    send_message( "550 "+command[1]+": Directory or file already exists.\n");
		} 
		else if (!check_valid_write(tmp))
		{
		    PERMISSION_DENIED550(command[ 1 ]);
		} 
		else 
		{
		    tmp2 = from_name;
		    if (!check_valid_write(tmp2))
		    {
			PERMISSION_DENIED550(tmp2);
			from_name = 0;
		    }
		    else
		    {
#ifdef FILE_LOCKING
			if (flock_wrapper(tmp, F_LOCK, fd)) {
			    if (flock_wrapper(tmp2, F_LOCK, fd)) {
#endif

#ifdef LOG_TIME
				log_file( LOG_FILE, FTP_TIME );
#endif

#ifdef LOG_FILE
				log_file( LOG_FILE, 
				          name+" rnfr "+tmp2+
				          " rnto "+tmp+".\n");
#endif
				if (catch( i = rename(tmp2, tmp) ) || i)
				    send_message("550 RNTO command failed.\n");
				else
				    send_message("250 RNTO command successful.\n");
#ifdef FILE_LOCKING
				flock_wrapper(tmp2, F_UNLOCK, fd);
				flock_wrapper(tmp, F_UNLOCK, fd);
				socket_info[fd][FROMNAME] = 0;
			    } else {
				PERMISSION_DENIED550(tmp2);
				flock_wrapper(tmp, F_UNLOCK, fd);
			    }
			} else {
			    PERMISSION_DENIED550(command[ 1 ]);
			}
#endif
		    }
		}
	    }
	    break;
	case "help": // give help information
	    if (sizeof(command) > 1)
	    {
		tmp = lower_case(command[1]);
		if (!cmdtab[ tmp ])
		{
		    misc = cmdtab[ tmp ];
		    if (misc[1])
			send_message("214 Syntax: "+misc[0]+" "+misc[2]+".\n");
		    else
			send_message("214 "+misc[0]+" "+misc[2]+"; unimplemented.\n");
		} 
		else
		{
		    send_message("502 Unknown command "+command[1]+".\n");
		}
	    }
	    else
	    {
		send_message( "214- The following commands are recognized "+
  		              "(* =>'s unimplemented).\n214-\n");
		misc = map_indices(cmdtab);
		s = sizeof(misc);
		
		tmp = "214- ";
		for (i = 0; i < s; i++)
		{
		    tmp += (cmdtab[misc[i]][1]) ? " " : "*"+
		           "           "[0..7-strlen(cmdtab[misc[i]][0])]+
  		           cmdtab[misc[i]][0];
		    if (i % 8 == 7)
		    {
			send_message( tmp + "\n");
			tmp = "214- ";
		    }
		}
		if (i % 8)
		    send_message( tmp + "\n");
		    
		send_message("214-\n214 Direct comments to "+FTP_BUGS_EMAIL+".\n");
	    }
	    break;
	case "acct": // specify account (ignored)
	case "mail": // mail to user
	case "mlfl": // mail file
	case "mode": // specify data transfer mode
	case "mrcp": // mail recipient
	case "mrsq": // mail recipient scheme question
	case "msnd": // mail send to terminal
	case "msam": // mail send to terminal and mailbox
	case "msom": // mail send to terminal or mailbox
	case "pasv": // prepare for server-to-server transfer
	case "rein": // reinitialize server state
	case "rest": // restart command/incomplete transfer
	case "smnt": // structure mount
	case "stru": // specify data transfer/file structure
	    send_message( "502 '"+command[0]+"' command not implemented.\n" );
	    break;

	default:
	    send_message( "500 '"+command[0]+"': command not understood.\n" );
	    break;
    }
} /* parse_com() */


/*
 * accept user command
 */
void receive_message( string str )
{
    string *command;
    int i, j;

    if (!str)  str = "";
    
    str = implode( explode( str, "\r"), "" );
    
    command = explode( str, "\n" ) - ({ 0 });
    
    j = sizeof(command);

    for ( i = 0; i < j; i++ )
	parse_comm( command[i] );
}

void close(int destr)
{
    logout();
    lose_user(1);

    FTPPORT->unregister_session(session_id);

    if(!destr)
        destruct_object();
}

/* EOF */
