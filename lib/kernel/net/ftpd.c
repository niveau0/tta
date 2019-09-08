/*
 *  Originally from TMI-2 1.2
 */

#include <access.h>
#include <config.h>
#include <filepath.h>
#include <ftpd.h>
#include <kernel.h>
#include <macros.h>
#include <net.h>

static int     timestamp, logged, session_id, data_mode, data_port;
static string  name, host, current_dir, data_ip, from_name;
static object  data_connect;
static mapping cmdtab, sitecmdtab;

static nomask void
create()
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

nomask void
init_client(int id, string ip, int port)
{
    if (previous_program() != FTPPORTOBJ)
	error("Illegal call to init_client");
    session_id = id;
    data_ip = ip;
    data_port = port;
}

nomask void
set_host(string h)
{
    if (previous_program() != FTPPORTOBJ)
	error("Illegal call to set_host");
    host = h;
}

private int
check_password(string name, string plaintext)
{
    string cpass;
    
    seteuid(_ROOT); /* first change back to uid */
    if (cpass = read_file(PLAYER_FILE(name) + ".o"))
	sscanf(cpass, "%*spassword \"%s\"\n%*s", cpass);
    seteuid("0"); /* now ftpd can change at will */
    if (!cpass)
        return 0;

    return (crypt(plaintext, cpass) == cpass);
}

private void
logout()
{
    logged = 0;
    seteuid("0");
#ifdef FTP_LOG
    if (name)
	syslog(FTP_LOG, BS(name + " logged out from " + host, 76, 1));
#endif
}

nomask int
_Q_idle()
{
    return time() - timestamp;
}

static nomask void
timeout()
{
    if (_Q_idle() > 900)
    {
	logout();
	send_message("421 Service closing control connection. Timeout.\n");
	destruct_object();
    }
    else
	call_out("timeout", 300);
}

static nomask void
open()
{
    if (!FTPPORTOBJ->check_connect(session_id))
    {
        send_message("530 Too many ftp connections active. " +
		     "Try again later.\n");
        destruct_object();
	return;
    }
    send_message("220 " + MUD_ABBREV +
		 " FTP server (Version " + FTPD_VERSION +
		 " [DGD/LPC]) ready.\n");
    call_out("timeout", 300);
}

nomask void
close(int flag)
{
    logout();

    FTPPORTOBJ->close_session(session_id);
    if (!flag)
	destruct_object();
}


private void
send_data(string data, string cmd, int type)
{
    string euid;
    
    if (data_connect)
    {
	send_message("425 Can't open data connection.\n");
	return;
    }
    data_connect = clone_object(FTPDATACONN);
    euid = geteuid();
    seteuid(_ROOT);
    seteuid(euid, data_connect);
    seteuid(euid);
    data_connect->send_data(this_object(), data, cmd, type,
			    data_mode, data_port, data_ip);
}

private void
receive_data(string path, string cmd, int append)
{
    string euid;
    
    if (data_connect)
    {
	send_message("425 Can't open data connection.\n");
	return;
    }
    data_connect = clone_object(FTPDATACONN);
    euid = geteuid();
    seteuid(_ROOT);
    seteuid(euid, data_connect);
    seteuid(euid);
    data_connect->receive_data(this_object(), path, cmd, append,
			       data_mode, data_port, data_ip);
} 

/*
 * parse user command
 */
static nomask void
parse_comm(string str) 
{
    int    i, sz;
    string path, *command;

    timestamp = time(); /* idle stamp */
    if (!sizeof(command = explode(str, " ")))
	return;
    
    switch(lower_case(command[0]))
    {
    case "port": /* specify data connection port */
	  command = explode(implode(command[1.. ], " "), ",");
	  if (sizeof(command) < 6)
	      send_message("550 Failed command.\n");
	  else
	  {
	      data_ip = implode(command[0..3], ".");
	      sscanf(command[4], "%d", i);
	      data_port = i<<8;
	      sscanf(command[5], "%d", i);
	      data_port += i;
	      send_message( "200 PORT command successful.\n" );
	  }
	  break;
      case "user": /* specify user name */
	  if (logged)
	  {
	      logout();
	      name = 0;
	  }
	  if (!_SYSTEMD->exist_player(command[1]))
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS("Login denied for " + name +
				 " from " + host, 76, 1));
#endif
	      send_message("530 User " + command[1] + " access denied.\n");
	  } 
	  else 
	  {
	      send_message("331 Password required for " + command[1] + ".\n");
	      name = command[1];
	  }
	  break;
    case "pass": /* specify password*/
	  if (!name) 
	      send_message("533 Log in with USER first.\n");
	  else if (logged) 
	      send_message("230 Already logged in.\n");
	  else if (!check_password(name, implode(command[1..], " ")))
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS("Login denied for " + name +
				 " from " + host, 76, 1));
#endif
	      send_message("530 Login incorrect.\n");
	      name = 0;
	  } 
	  else
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " connected from " + host, 76, 1));
#endif
	      logged = 1;
	      current_dir = "/home/" + name;
	      if (file_size(current_dir) != -2)
	      {
		  send_message("230 No home directory!\n");
		  current_dir = "/";
	      }
	      else 
	      {
#ifdef FTP_LOGIN_MSG
		  send_message("230- " + FTP_LOGIN_MSG + "\n");
#endif
		  send_message("230 User " + name +
			       " logged in successfully.\n");
	      }
	      data_mode = ASCII;
	      seteuid(_ROOT); /* first change back to uid */
	      seteuid(name); /* now ftpd can change at will */
	  }
	  break;
    case "noop": /* do nothing */
	  send_message( "200 NOOP command successful.\n" );
	  break;
    case "retr": /* retrieve a file */
	  LOGGED;	  
	  path = FPATH(current_dir, command[1]);
	  if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else if ((i = file_size(path)) == -2)
	      send_message("550 " + command[1] + ": Not a plain file.\n");
	  else if (i == -1) 
	      send_message("550 " + command[1] +
			   ": No such file or directory.\n");
	  else 
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " retr " + path +
				 " (" +
				 (data_mode == BINARY ? "binary" : "ascii") +
				 ")", 76, 1));
#endif
	      send_data(path, command[1], FILE);
	  }
	  break;
    case "stou": /* store a file with a unique name */
	  LOGGED;	  
	  path = FPATH(current_dir, command[1]);
	  if ((i = file_size(path)) == -2 ||
	      !_ACCESS_WRITE(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  
	  for (i=MAX_UNIQUE_TRIES; i--;)
	  {
	      if (file_size(path+i) == -1)
		  break;
	  }
	  if (i)
	  {
	      path += i;
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " stou " + path +
				 " (" +
				 (data_mode == BINARY ? "binary" : "ascii") +
				 ")", 76, 1));
#endif
	      receive_data(path, command[1], NO_APPEND);
	  }
	  else
	      send_message("452 Unique file name cannot be created.\n");
	  break;
    case "stor": /* store a file */
	  LOGGED;	  
	  path = FPATH(current_dir, command[1]);
	  if (file_size(path) == -2 ||
	      !_ACCESS_WRITE(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " stor " + path +
				 " (" +
				 (data_mode == BINARY ? "binary" : "ascii") +
				 ")", 76, 1));
#endif
	      receive_data(path, command[1], NO_APPEND);
	  }
	  break;
    case "appe": /* append to a file */
	  LOGGED;
	  path = FPATH(current_dir, command[1]);
	  if (file_size(path) < 0 ||
	      !_ACCESS_WRITE(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " appe " + path +
				 " (" +
				 (data_mode == BINARY ? "binary" : "ascii") +
				 ")", 76, 1));
#endif
	      receive_data(path, command[1], APPEND);
	  }
	  break;
    case "dele": /* delete a file */
	  LOGGED;
	  path = FPATH(current_dir, command[1]);
	  if ((i = file_size(path)) == -2)
	      send_message("550 " + command[1] + ": Not a plain file.\n");
	  else if (i == -1)
	      send_message("550 " + command[1] +
			   ": No such file or directory.\n");
	  else if (_ACCESS_WRITE(path, this_object(), "parse_comm"))
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " dele " + path, 76, 1));
#endif
	      if (remove_file(path))
		  send_message("250 DELE command successful.\n");
	      else
		  send_message("550 Permission denied.\n");
	  }
	  else
	      send_message("550 Permission denied.\n");
	  break;
    case "mdtm": /* show last modification time of file */
	  LOGGED;	  
	  /* Supposed to return modified time in the format: YYYYMMDDHHMMSS */
	  path = FPATH(current_dir, command[1]);
	  if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else if ((i = file_size(path)) == -2)
	      send_message("550 " + command[1] + ": Not a plain file.\n");
	  else if (i == -1)
	      send_message("550 " + command[1] +
			   ": No such file or directory.\n");
	  else
	  {
	      string tstr, zero;
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " mdtm " + path, 76, 1));
#endif
	      tstr = ctime(file_time(path));
	      i = member_array(tstr[4..6], MONTHS) + 1;
	      if (i < 10)
		  zero = "0";
	      else
		  zero = "";
	      tstr = (tstr[20..23] + zero + i +
		      (tstr[8] == ' ' ? "0" + tstr[8..8] : tstr[8..9]) +
		      tstr[11..12] + tstr[14..15] + tstr[17..18]);
	      send_message("213 " + tstr + "\n");
	  }
	  break;
    case "size": /* return size of file */
	  LOGGED;
	  path = FPATH(current_dir, command[1]);
	  if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else if ((i = file_size(path)) == -2)
	      send_message("550 " + command[1] + ": Not a plain file.\n");
	  else if (i == -1)
	      send_message( "550 " + command[1] + " does not exist.\n");
	  else
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " size " + path, 76, 1));
#endif
	      send_message( "215 " + file_size(path) + "\n");
	  }
	  break;
    case "nlst": /* give name list of files in directory */
    case "list": /* list files in a directory */
      {
	  string opt, list;
	      
	  LOGGED;
	  opt = "";
	  if ((sz = sizeof(command)) < 2 || !strlen(command[1]))
	      path = current_dir;
	  else if (command[1][0] != '-')
	      path = FPATH(current_dir, command[1]);
	  else if (sz == 2) /* only option, no path */
	  {
	      opt = command[1];
	      path = current_dir;
	  }
	  else
	  {
	      opt = implode(command[1..sz-2], " ");
	      path = FPATH(current_dir, command[sz-1]);
	  }
	  if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else
	  {
	      list = _LS((opt == "" ? "-1" : opt) + " " + path);
	      if (!strlen(list))
		  send_message("550 No files found.\n");
	      else
		  send_data(list, "ls", STRING);
	  } 
	  break;
      }
    case "xpwd": /* print the current working directory (deprecated) */
    case "pwd":  /* print the current working directory */
	  LOGGED;
	  send_message("257 \"" + current_dir + "\" is current directory.\n");
	  break;
    case "xcup": /* change to parent of working directory (deprecated) */
    case "cdup": /* change to parent of current working directory */
	  if (sizeof(command) > 1)
	      command[1] = "..";
	  else
	      command += ({ ".." });
    case "xcwd": /* change working directory (deprecated) */
    case "cwd":  /* change working directory */
	  LOGGED;	  
	  if (sizeof(command) > 1)
	      path = FPATH(current_dir, command[1]);
	  else
	      path = current_dir;
	  
	  if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 " + path + ": No such file or directory.\n");
	  else
	  {
	      if (file_size(path) != -2)
	      {
		  send_message("550 " + path + ": Not a directory.\n" );
		  break;
	      }
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " cwd " + path, 76, 1));
#endif
	      current_dir = path;
	      send_message("250 " + command[0] + " command successful.\n");
	  }
	  break;
    case "quit": /* terminate session */
	  send_message("221 Goodbye.\n");
	  logout();
	  break;
    case "type": /* specify data transfer type */
	  LOGGED;
	  if (command[1] == "I")
	  {
	      data_mode = BINARY;
	      send_message("200 Type set to I.\n");
	  }
	  else if (command[1] == "A") 
	  {
	      data_mode = ASCII;
	      send_message("200 Type set to A.\n");
	  } 
	  else
	      send_message("504 Type " + command[1] + " not implemented.\n");
	  break;
    case "stat": /* return status of server */
	send_message("211 FTP server status: " + MUD_NAME +
		       " MUD FTP Version " + FTPD_VERSION + "\r\n"+
		     "     Connected to " + host + "\r\n");
	  if (logged && name)
	      send_message("    Logged in as " + name + "\r\n");
	  else if (name)
	      send_message("    Waiting for password.\r\n");
	  else
	      send_message("    Waiting for user name.\r\n");
	  
	  send_message("    TYPE: ASCII, FORM: Nonprint; STRUcture: " +
		       "File; transfer MODE: Stream.\r\n");
	  if (data_connect)
	      send_message("    Data connection open.\r\n");
	  else if (data_ip)
	      send_message( "    " + data_ip + " " + data_port + "\r\n");
	  else
	      send_message( "    No data connection.\r\n" );
	  
	  send_message("211 End of status.\n");
	  break;
    case "abor": /* abort previous command */
	  if (data_connect)
	  {
	      send_message("426 Transfer aborted. Data connection closed.\n");
	      destruct_object();
	  }
	  send_message("226 Abort successful.\n");
	  break;
    case "syst": /* show operating system type of server system */
	  send_message("215 DGD\n");
	  break;
    case "xmkd": /* make a directory (deprecated) */
    case "mkd":  /* make a directory */
	  LOGGED;
	  path = FPATH(current_dir, command[1]);
	  if (file_size(path) != -1)
	      send_message("550 " + command[1] +
			   ": Directory or file already exists.\n");
	  else if (!_ACCESS_WRITE(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else if (make_dir(path))
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " mkd " + path, 76, 1));
#endif
	      send_message("257 " + command[0] + " command successful.\n");
	  }
	  else
	      send_message("550 " + command[0] + " command failed.\n");
	  break;
    case "xrmd": /* remove a directory (deprecated) */
    case "rmd":  /* remove a directory */
	  LOGGED;	  
	  path = FPATH(current_dir, command[1]);
	  if (file_size(path) != -2)
	      send_message("550 " + command[1] +
			   ": No such file or directory.\n");
	  else if (!_ACCESS_WRITE(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else if (remove_dir(path))
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " rmd " + path, 76, 1));
#endif
	      send_message("250 " + command[0] + " command successful.\n");
	  } 
	  else
	      send_message("550 " + command[0] + " command failed.\n");
	  break;
      case "rnfr": /* specify rename-from file name */
	  LOGGED;
	  path = FPATH(current_dir, command[1]);
	  if ((i = file_size(path)) == -2)
	      send_message("550 " + command[1] + ": Not a plain file.\n");
	  else if (i == -1)
	      send_message("550 " + command[1] +
			   ": No such file or directory.\n");
	  else if (!_ACCESS_READ(path, this_object(), "parse_comm"))
	      send_message("550 Permission denied.\n");
	  else
	  {
	      from_name = path;
	      send_message("350 File exists, ready for destination name.\n");
	  }
	  break;
      case "rnto": /* specify rename-to file name */
	  LOGGED;
	  if (!from_name)
	  {
	      send_message("503 Bad sequence of commands.\n");
	      break;
	  }
	  path = FPATH(current_dir, command[1]);

	  if ((i = file_size(path)) != -1)
	      send_message("550 " + command[1] +
			   ": Directory or file already exists.\n");
	  else if (!_ACCESS_WRITE(path, this_object(), "parse_comm") ||
		   !_ACCESS_WRITE(from_name, this_object(), "parse_comm"))
	  {
	      send_message("550 Permission denied.\n");
	      from_name = 0;
	  }
	  else
	  {
#ifdef FTP_LOG
	      syslog(FTP_LOG, BS(name + " rnfr " + from_name + " rnto " +
				 path, 76, 1));
#endif
	      if (!rename_file(from_name, path))
		  send_message("550 RNTO command failed.\n");
	      else
		  send_message("250 RNTO command successful.\n");
	  }
	  break;
      case "help": /* give help information */
      {
	  string list, *ind;
	  mixed  *help;
	  
	  if (sizeof(command) > 1)
	  {
	      if (!(help = cmdtab[lower_case(command[1])]))
		  send_message("502 Unknown command " + command[1] + ".\n");
	      else if (help[1])
		  send_message("214 Syntax " + help[0] + " " +
			       help[2] + ".\n");
	      else
		  send_message("214 " + help[0] + " " + help[2] +
			       "; not implemented.\n");
	  }
	  else
	  {
	      send_message("214- The following commands are recognized "+
			   "(* => not implemented).\n214-\n");
	      ind = map_indices(cmdtab);
	      list = "214-\n214- ";
	      for (i=0, sz=sizeof(ind); i<sz; i++)
	      {
		  help = cmdtab[ind[i]];
		  list += "           "[0..7 - strlen(help[0])] +
			  (help[1] ? " " : "*") + help[0];
		  if (i%8 == 7)
		  {
		      send_message(list + "\n");
		      list = "214- ";
		  }
	      }
	      if (i%8)
		  send_message(list + "\n");
	      
	      send_message("214-\n214 Direct comments to admin of " +
			   MUD_NAME + ".\n");
	  }
	  break;
      }
      case "acct": /* specify account (ignored) */
      case "mail": /* mail to user */
      case "mlfl": /* mail file */
      case "mode": /* specify data transfer mode */
      case "mrcp": /* mail recipient */
      case "mrsq": /* mail recipient scheme question */
      case "msnd": /* mail send to terminal */
      case "msam": /* mail send to terminal and mailbox */
      case "msom": /* mail send to terminal or mailbox */
      case "pasv": /* prepare for server-to-server transfer */
      {
	  send_message( "202 " + command[0] + " command not implemented.\n");
	  /*string euid;
	  data_connect = clone_object(FTPDATACONN);
	  euid = geteuid();
	  seteuid(_ROOT);
	  seteuid(euid, data_connect);
	  seteuid(euid);
	  send_message("227 Entering Passive Mode ().\n");*/
	  break;
      }
      case "rein": /* reinitialize server state */
      case "rest": /* restart command/incomplete transfer */
      case "smnt": /* structure mount */
      case "stru": /* specify data transfer/file structure */
      case "allo": /* allocate storage (vacuously) */
      case "site": /* non-standard commands */
	  send_message( "202 " + command[0] + " command not implemented.\n");
	  break;
      default:
	  send_message( "500 Syntax error, command unrecognized.\n");
	  break;
    }
}

nomask void
receive_message(string str)
{
    int    i, sz;
    string *command;
    
    if (!str)
	return;
    set_this_player(0);
    str = implode(explode(str, "\r"), "");
    command = explode(str, "\n");
    for (i=0, sz=sizeof(command); i<sz; i++) 
	parse_comm(command[i]);
}

nomask void
dataconn_fail()
{
    if (previous_object() != data_connect)
	error("Illegal call to dataconn_fail");
    send_message("425 Can't build data connection.\n");
}

nomask void
dataconn_uopened(int len, string name, int type)
{
    if (previous_object() != data_connect) 
	error("Illegal call to dataconn_uopened");
    send_message("150 Opening " +
		 (type == ASCII ? "ASCII" : "BINARY") +
		 " mode data connection for " + name +
		 " (" + len + " bytes).\n");
    remove_call_out("timeout");
}

nomask void
dataconn_dopened(string name, int type)
{
    if (previous_object() != data_connect)
	error("Illegal call to dataconn_dopened");
    send_message("150 Opening " +
		 (type == ASCII ? "ASCII" : "BINARY") +
		 " mode data connection for " + name + ".\n");
    remove_call_out("timeout");
}

nomask void
dataconn_tcomplete()
{
    if (previous_object() != data_connect) 
	error("Illegal call to dataconn_tcomplete");
    send_message("226 Transfer complete.\n");
    call_out("timeout", 900);
}
