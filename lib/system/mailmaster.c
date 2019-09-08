#include <config.h>
#include <macros.h>

#define OPT_C 1 /* ask Cc */
#define OPT_M 2 /* use more while displaying */
#define OPT_H 4 /* show header */

#define RETURN_TO_INPUT 1
#define DATALENGTH      6

#define MAIL_TIME       0
#define MAIL_AUTHOR     1
#define MAIL_SUBJECT    2
#define MAIL_MSG        3
#define MAIL_TO         4
#define MAIL_STATE      5

#define CURRENT_READER  "_tmp_mail_reader"

static int     current, mode;
static string  pldir, text, folder, subject, *recipients;
static object  player;
static mapping global_alias; 

int     options;
mapping alias, allmails;

static nomask void
create()
{
    if (!IS_CLONE)
	return;
    
    if (!(player = this_player()) || !query_ip_number(player))
    {
	destruct_object();
	return;
    }
    call_out("clean_up", 180);

    pldir = PLAYER_DIR(player->query_name());
    
    if (file_size(pldir) == -2)
	restore_object(pldir + "/maildata");
    
    if (!mappingp(allmails))
	allmails = ([ "inbox":({ }) ]);
    
    if (!mappingp(alias))
	alias = ([ ]);
    
    global_alias = ([ "admin" : ({ "manwe", "ulmo", "irmo" }) ]);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}

static nomask void
clean_up()
{
    if (!player || !query_ip_number(player))
    {
	destruct_object();
	return;
    }
    call_out("clean_up", 180);
}

static nomask string *
resolve_alias(string *copy_to)
{
    int     i, j, sz, sz2;
    string  *c;
    mapping copies;

    if (!pointerp(copy_to))
	return ({ });
    
    copies = ([ ]);
    for (i=0, sz=sizeof(copy_to); i<sz; i++)
	if (c = alias[copy_to[i]])
	{
	    for (j=0, sz2=sizeof(c); j<sz2; j++)
		copies[c[j]] = 1;
	}
	else if (c = global_alias[copy_to[i]])
	{
	    for (j=0, sz2=sizeof(c); j<sz2; j++)
		copies[c[j]] = 1;
	}
	else
	    copies[copy_to[i]] = 1;

    return map_indices(copies);
}

nomask void
input_line()
{
    int sz;
    
    if (sz = sizeof(allmails[folder]))
	player->catch_tell(folder + " 1.." + sz + ", " + current + " " +
			   allmails[folder][current-1][MAIL_STATE] + ": ", 1);
    else
	player->catch_tell(folder + " (No mail): ", 1);
    input_to("get_input");
}

nomask void
check_mail(string name, object ob)
{
    if (!name || !ob || MASTER_OB(previous_object()) != _LOGIN)
	return;
    if (sizeof(get_dir(PLAYER_DIR(name) + "/new_*")))
        ob->catch_tell("\n                  _______   _______    \n"+
		       "                 / \\     `--`      \\   \n"+
                       "                 \\_|               |   \n"+
                       "                   >      New     /   \n"+ 
                       "                   |    Mail for  \\  \n"+ 
                       "                    /     you!     |   \n"+ 
                       "                   | ___  _________|_  \n"+ 
                       "                   | \\ _\\/__  _____ _\\ \n"+ 
                       "                   \\/_______________/ \n\n");
}

static nomask void
distribute_mail(string *copy_to)
{
    int     i, sz, num;
    string  to, dir, mail;
    object  m, pl, reader;
    mapping copies;
    
    m       = find_object(_SYSTEMD);
    copy_to = recipients + resolve_alias(copy_to);
    mail    = (time() + "@@@" + player->query_name() + "@@@" +
	       subject + "@@@" + text + "@@@" + implode(copy_to, "*") +
	       "@@@N");
    
    for (i=0, sz=sizeof(copy_to); i<sz; i++)
    {
	if (!m->exist_player(to = copy_to[i]))
	    continue;
	if (file_size(dir = PLAYER_DIR(to)) != -2)
	    make_dir(dir);
	num = sizeof(get_dir(dir + "/new_*"));
	write_file(dir + "/new_" + (num + 1), mail);
	if (pl = find_player(to))
	{
	    if (reader = pl->query_prop(CURRENT_READER))
	    {
		pl->catch_tell("\n- New mail has arrived.\n\n", 1);
		reader->get_new_mail();
	    }
	    else
		pl->catch_tell("\n- New mail has arrived.\n\n");
	}
    }
    player->catch_tell("Sent mail to:\n" +
		       BS(implode(copy_to, ", "), 75, 1) + ".\n", 1);
    if (mode == RETURN_TO_INPUT)
    {
	subject = 0;
	input_line();
	return;
    }
    player->catch_tell("");
    destruct_object();
}

static nomask void
ask_cc(string str)
{
    if (!str)
	distribute_mail(0);
    else
	distribute_mail(explode(str, " "));
}

static nomask void
set_options(string str)
{
    int    i, sz;
    string *opt;

    if (!str || !strlen(str))
    {
	str = "";
	if (options & OPT_C)
	    str += "C";
	if (options & OPT_H)
	    str += "H";
	if (options & OPT_M)
	    str += "M";
	player->catch_tell("Current options set: " + str + "\n", 1);
    }
    else
    {
	opt = explode(str, "");
	
	if (IN_ARRAY("C", opt))
	    options ^= OPT_C;
	if (IN_ARRAY("H", opt))
	    options ^= OPT_H;
	if (IN_ARRAY("M", opt))
	    options ^= OPT_M;
	
	player->catch_tell("Ok.\n", 1);
	save_object(pldir + "/maildata");
    }
    input_line();
}

static nomask void
get_subject(string str)
{
    if (!str || !strlen(str))
    {
	if (mode == RETURN_TO_INPUT)
	{
	    player->catch_tell("No subject. Cancelled.\n", 1);
	    input_line();
	}
	else
	{
	    player->catch_tell("No subject. Cancelled.\n");
	    destruct_object();
	}
	return;
    }
    subject = str;
    EDIT("finish_send", 0);
}

nomask void
finish_send(string t)
{
    if (!t || !strlen(t))
    {
	if (mode == RETURN_TO_INPUT)
	{
	    player->catch_tell("Cancelled.\n", 1);
	    input_line();
	}
	else
	{
	    player->catch_tell("Cancelled.\n");
	    destruct_object();
	}
	return;
    }
    text = t;
    if (options & OPT_C)
    {
	player->catch_tell("Cc: ", 1);
	input_to("ask_cc");
	return;
    }
    distribute_mail(0);
}

nomask void
get_new_mail()
{
    int    i, sz;
    string *maildata, *newmail;

    newmail = get_dir(pldir + "/new_*");
    
    if (!(sz = sizeof(newmail)))
    {
	player->catch_tell("No new mail has arrived.\n", 1);
	return;
    }
    for (i=0; i<sz; i++)
    {
	maildata = explode(read_file(pldir + "/" + newmail[i]), "@@@");
	if (sizeof(maildata) == DATALENGTH)
	    allmails["inbox"] += ({ maildata });
    }
    save_object(pldir + "/maildata");
    for (i=0; i<sz; i++)
	remove_file(pldir + "/" + newmail[i]);
}

static nomask void
send_mail(string r)
{
    recipients = resolve_alias(explode(r, " "));
    if (!sizeof(recipients) || (sizeof(recipients) == 1 &&
				!_SYSTEMD->exist_player(recipients[0])))
    {
	if (mode == RETURN_TO_INPUT)
	{
	    player->catch_tell("No such player.\n", 1);
	    input_line();
	}
	else
	    player->catch_tell("No such player.\n");
	return;
    }
    player->catch_tell("Subject: ", 1);
    input_to("get_subject");
}

static nomask void
reply_to_mail()
{
    int    sz;
    string out, *mail, **actfolder;

    actfolder = allmails[folder];
    if (!(sz = sizeof(actfolder)))
    {
	player->catch_tell("You have no mail in this folder.\n", 1);
	input_line();
	return;
    }
    if (current > sz || current < 1)
    {
	player->catch_tell("No mail selected to reply to.\n", 1);
	input_line();
	return;
    }
    mail = actfolder[current-1];
    recipients = ({ mail[MAIL_AUTHOR] });
    subject = ">" + mail[MAIL_SUBJECT];
    player->catch_tell(BS("You reply to: " + implode(recipients, " "), 76, 1) +
		       "\nSubject: " + subject + "\n");
    EDIT("finish_send", 0);
}

static nomask void
forward_mail(string r)
{
    int    sz;
    string out, *mail, **actfolder;

    actfolder = allmails[folder];
    if (!(sz = sizeof(actfolder)))
    {
	player->catch_tell("You have no mail in this folder.\n", 1);
	input_line();
	return;
    }
    if (current > sz || current < 1)
    {
	player->catch_tell("No mail selected to forward.\n", 1);
	input_line();
	return;
    }
    recipients = resolve_alias(explode(r, " "));
    if (!sizeof(recipients) || (sizeof(recipients) == 1 &&
				!_SYSTEMD->exist_player(recipients[0])))
    {
	player->catch_tell("No such player.\n", 1);
	input_line();
	return;
    }
    mail = actfolder[current-1];
    subject = mail[MAIL_SUBJECT] + " (fwd)";
    text = mail[MAIL_MSG];
    if (options & OPT_C)
    {
	player->catch_tell("Cc: ", 1);
	input_to("ask_cc");
	return;
    }
    distribute_mail(0);
}

static nomask void
group_reply()
{
    int    sz;
    string out, *mail, **actfolder;

    actfolder = allmails[folder];
    if (!(sz = sizeof(actfolder)))
    {
	player->catch_tell("You have no mail in this folder.\n", 1);
	input_line();
	return;
    }
    if (current > sz || current < 1)
    {
	player->catch_tell("No mail selected to reply to.\n", 1);
	input_line();
	return;
    }
    mail = actfolder[current-1];
    recipients = ({ mail[MAIL_AUTHOR] }) | explode(mail[MAIL_TO], "*");
    subject = ">" + mail[MAIL_SUBJECT];
    player->catch_tell(BS("You reply to: " + implode(recipients, " "), 76, 1) +
		       "\nSubject: " + subject + "\n");
    EDIT("finish_send", 0);
}

static nomask varargs void
list_mail(int flag)
{
    int    i, sz;
    string out, *mail, **actfolder;

    actfolder = allmails[folder];
    if (!(sz = sizeof(actfolder)))
    {
	if (mode != RETURN_TO_INPUT)
	{
	    player->catch_tell("You have no mail.\n");
	    return;
	}
	player->catch_tell("You have no mail in this folder.\n", 1);
	input_line();
	return;
    }
    out = "";
    for (i=0; i<sz; i++)
    {
	mail = actfolder[i];
	if (flag && mail[MAIL_STATE] != "N")
	    continue;
	out += sprintf(" %3d  %s  %-15s  %-10s  %-40s\n",
		       i + 1,
		       mail[MAIL_STATE],
		       ctime((int)mail[MAIL_TIME])[4..18],
		       mail[MAIL_AUTHOR],
		       mail[MAIL_SUBJECT]);
    }
    if (!flag || out != "")
	out = "\n Nr.  S  Date             Author      Subject\n-" +
	   "====================================================" +
	   "=========================-\n" + out;
    
    if (mode != RETURN_TO_INPUT)
    {
	if (options & OPT_M)
	    smore(out, player);
	else
	    player->catch_tell(out, 1);
	return;
    }
    if (options & OPT_M)
	smore(out, player, "input_line");
    else
    {
	player->catch_tell(out, 1);
	input_line();
    }
}

static nomask void
display_mail(int flag) /* if flag is true, current won't be increased */
{
    int    sz;
    string out, *mail, **actfolder;

    actfolder = allmails[folder];
    if (!(sz = sizeof(actfolder)))
    {
	player->catch_tell("You have no mail in this folder.\n", 1);
	input_line();
	return;
    }

    if (current > sz)
	current = sz;
    else if (current < 1)
	current = 1;
    
    mail = actfolder[current-1];
    if (!flag)
    {
	int curr;

	curr = current;
	while (mail[MAIL_STATE] != "N")
	{
	    if (++curr > sz)
	    {
		player->catch_tell("No new mail in this folder.\n", 1);
		input_line();
		return;
	    }
	    mail = actfolder[curr-1];
	}
	current = curr;
    }
    if (mail[MAIL_STATE] == "N")
	mail[MAIL_STATE] = "o";
    
    out = "";
    if (options & OPT_H)
    {
	out += ("--------------------------------------------------------"+
		"-----------------------" +
		"\n Date:    " + ctime((int)mail[MAIL_TIME]) +
		"\n Author:  " + mail[MAIL_AUTHOR] +
		"\n Subject: " + mail[MAIL_SUBJECT] +
		"\n To:      " + BS(implode(explode(mail[MAIL_TO], "*"),
					    ", "), 65, 10)[10..] +
		"\n--------------------------------------------------------"+
		"-----------------------\n");
    }
    if (options & OPT_M)
	smore(out + mail[MAIL_MSG], player, "input_line");
    else
    {
	player->catch_tell(out + mail[MAIL_MSG], 1);
	input_line();
    }

    save_object(pldir + "/maildata");
}

static nomask void
delete_mail(string arg)
{
    int    from, to;
    string **actfolder;

    actfolder = allmails[folder];
    if (!arg || !strlen(arg))
    {
	if (current > sizeof(actfolder))
	{
	    player->catch_tell("No such mail.\n", 1);
	    return;
	}
	actfolder[current-1][MAIL_STATE] = "D";
    }
    else if (sscanf(arg, "%d-%d", from, to) == 2)
    {
	if (from > to)
	{
	    player->catch_tell("That is not possible.\n", 1);
	    return;
	}
	if (to > sizeof(actfolder))
	    to = sizeof(actfolder);
	for (; from <= to; from++)
	    actfolder[from-1][MAIL_STATE] = "D";
	return;
    }
    else if (sscanf(arg, "%d", from))
    {
	if (from > sizeof(actfolder))
	    from = sizeof(actfolder);
	actfolder[from-1][MAIL_STATE] = "D";
    }
    else
	player->catch_tell("Wrong argument.\n", 1);
    save_object(pldir + "/maildata");
}

static void
perform_deletion()
{
    int    i, sz;
    string **actfolder;
    
    actfolder = allmails[folder];
    for (i=0, sz=sizeof(actfolder); i<sz; i++)
	if (actfolder[i][MAIL_STATE] == "D")
	    actfolder[i] = 0;
    allmails[folder] = actfolder - ({ 0 });
    if (current > (sz = sizeof(allmails[folder])))
	current = sz;
    save_object(pldir + "/maildata");
}

static void
delete_yesno(string str)
{
    int    i, sz;
    string **actfolder;
	
    if (str == "y" || str == "yes")
	perform_deletion();
    player->catch_tell("Ok.\n");
    destruct_object();
}

static void
quit_check()
{
    int    i, sz;
    string **actfolder;
    
    actfolder = allmails[folder];
    for (i=0, sz=sizeof(actfolder); i<sz; i++)
	if (actfolder[i][MAIL_STATE] == "D")
	    break;
    if (i != sz)
    {
	player->catch_tell("Do you want to delete the marked mails? ", 1);
	input_to("delete_yesno");
	return;
    }
    player->catch_tell("Ok.\n");
    destruct_object();
}

static void
get_input(string str)
{
    int    num;
    string arg;
    
    if (!str || !strlen(str))
    {
	display_mail(0);
	return;
    }
    sscanf(str, "%s %s", str, arg);
    switch(str)
    {
      case "?":
	  player->catch_tell("m <arg>     Send a mail\n" +
			     "r           Reply to current mail\n" +
			     "f <to>      Forward selected mail to someone\n" +
			     "g           Group reply to current mail\n" +
			     "l           List mail\n" +
			     "o           Set options, possible: C(opy) " +
			     "H(eader) M(ore)\n" +
			     "d <num-num> Mark mail for deletion\n" +
			     "#           Delete marked mail\n" +
			     "<num>       Read mail number <num>\n" +
			     "<cr>        Show next mail\n" +
			     "q           Quit\n", 1);
	  input_line();
	  return;
      case "m":
      case "mail":
	  if (!arg)
	  {
	      player->catch_tell("Mail to whom?\n", 1);
	      input_line();
	      return;
	  }
	  send_mail(arg);
	  return;
      case "r":
	  reply_to_mail();
	  return;
      case "f":
	  forward_mail(arg);
	  return;
      case "g":
	  group_reply();
	  return;
      case "l":
	  list_mail();
	  return;
      case "o":
	  set_options(arg);
	  return;
      case "d":
	  delete_mail(arg);
	  input_line();
	  return;
      case "#":
	  perform_deletion();  
	  input_line();
	  return;
      case "q":
	  quit_check();
	  return;
    }
    if (sscanf(str, "%d", num))
    {
	current = num;
	display_mail(1);
	return;
    }
    player->catch_tell("No such command, use '?' for help.\n", 1);
    input_line();
}

/* called from cmdsoul */
nomask void
read_mail(string arg)
{
    if (arg)
    {
	send_mail(arg);
	return;
    }
    mode    = RETURN_TO_INPUT;
    current = 1;
    folder  = "inbox";
    player->add_prop(CURRENT_READER, this_object());
    player->catch_tell(" *------------------------------"+
		       "---------------------------------------------*\n"+
		       sprintf("%|77s\n", "'" + MUD_NAME + "' mail") +
		       " *------------------------------"+
		       "---------------------------------------------*\n", 1);
    if (file_size(pldir) == -2)
	get_new_mail();
    list_mail(1); /* show new mail */
}

/* called from cmdsoul */
nomask void
from_cmd(object ob)
{
    if (!ob || object_name(previous_object()) != MISC_SOUL)
	return;
    mode   = 0;
    folder = "inbox";
    player = ob;
    pldir  = PLAYER_DIR(player->query_name());
    if (file_size(pldir) == -2)
	restore_object(pldir + "/maildata");
    if (!mappingp(allmails))
	allmails = ([ "inbox":({ }) ]);
    if (file_size(pldir) == -2)
	get_new_mail();
    list_mail();
    allmails = 0;
    player = 0;
    pldir = 0;
}
