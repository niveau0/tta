/*
 * login.c is called if people connect at the telnet port. 
 */
#include <access.h>
#include <macros.h>
#include <language.h>
#include <kernel.h>
#include <net.h>
#include <status.h>
#include <properties.h>

static nomask void start_player(int i);

string         name,           /* The real name of the player */
               password,       /* The password of the player */
               race_name;      /* The race */
static object  user;

static nomask void
create()
{
    if (IS_CLONE)
	call_out("clean_up", 150);
}

nomask varargs void
catch_tell(string msg, int no_prompt)
{
    if (user)
	user->catch_tell(msg, no_prompt);
}

nomask void
set_user(object u)
{
    if (u && program_name(previous_object()) != _USER)
	return;
    user = u;
}

nomask object
query_user()
{
    return user;
}

/*
 * Function name: clean_up
 * Description:   remove login object if connection was lost or user is idle
 */
static nomask void
clean_up()
{
    if (!query_ip_number())
    {
	if (user)
	    user->close();
	destruct_object();
    }
    else if (!user)
	destruct_object();
    else if (query_idle(this_object()) > 300)
    {
	catch_tell("\nTime out. You have been idle too long.\n", 1); 
	user->close();
	destruct_object();
    }
    else
	call_out("clean_up", 150);
}

/*
 * Function name: notify_coders
 * Description:   notify all coders that a new player enters the game
 * Arguments:     player - the player that is login in
 *                flag - if -1, someone connected
 *                       if 0, player entered game
 *                       if 1, player was linkdead and relogged
 *                       if 2, player left linkdead
 *                       if 3, player left game
 */
nomask void
notify_coders(object player, int flag)
{
    int    i;
    string msg, n;
    object *u;

    switch (flag)
    {
      case -1:
	  msg = "[connection attempt - from " +
		query_ip_number(player) + "]\n";
	  break;
      case 0:
	  if (!(n = name))
	      n = "'noname'";
	  msg = "[" + capitalize(n) + " - enters from " +
		query_ip_number(player) + "]\n";
	  break;
      case 1:
	  if (!(n = name))
	      n = "'noname'";
	  msg = "[" + capitalize(n) + " - reenters from " +
		query_ip_number(player) + "]\n";
	  break;
      case 2:
	  if (!(n = player->query_name()))
	  {
	      if (player == this_object())
		  n = "logon";
	      else
		  n = "'noname'";
	  }
	  msg = "[" + capitalize(n) + " - linkdead]\n";
	  break;
      case 3:
	  if (!(n = player->query_name()))
	  {
	      if (player == this_object())
		  n = "logon";
	      else
		  n = "'noname'";
	  }
	  msg = "[" + capitalize(n) + " - logged out]\n";
	  break;
    }
    for (i=sizeof(u=users()); i--;)
    {
        if (_ACCESS_LEVEL(geteuid(u[i])) && u[i]->query_prop(CODER_MSG_ENTER))
	    u[i]->catch_tell(msg);
    }
}

/*
 * Function name: logon
 * Description:   called by _USER if someone connects
 */
nomask void
logon()
{
    if (!query_ip_number())
    {
	destruct_object();
	return;
    }
    if (_SYSTEMD->check_newplayer(query_ip_number()) == 1)
    {
	catch_tell("Your site is banished. Hasta La Vista, Baby!\n", 1);
	destruct_object();
	return;
    }
    cat(LOGIN_FILE_WELCOME);
    catch_tell(sprintf("\n Driver: DGD-%-7s", status()[ST_VERSION]) +
	       "(using net package)             " +
	       "Mudlib Version: " + MUDLIB_VERSION +
	       "\n\nWhat should be your characters name: ", 1); 
    notify_coders(this_object(), -1);
    input_to("get_name");
}

/*
 * Function name: valid_name
 * Description:   check if the name is allowed (only lowercase and letters)
 * Arguments:     str - the entered name
 * Returns:       1 if name is valid, 0 else
 */
static nomask int
valid_name(string str)
{
    int i, length;
    
    if (!str || (length = strlen(str)) < 2 || str == _ROOT)
    {
	catch_tell("Sorry, that name is invalid.\n", 1);
	return 0;
    }
    if (length > 11)
    {
	catch_tell("Sorry, that name is too long.\n", 1);
	return 0;
    }
    for (i=0; i<length; i++)
    {
	if (str[i] < 'a' || str[i] > 'z')
	{
	    catch_tell("Invalid characters in name: " + str +
		       "\nCharacter index was " + (i + 1) + ".\n", 1);
	    return 0;
	}
    }
    return 1;
}

/*
 * Function name: get_name
 * Description:   receive player name
 * Arguments:     str - name
 */
static nomask void
get_name(string str)
{
    string file;
    
    if (!str || !strlen(str) || !query_user())
    {
	destruct_object();
	return;
    }
    str = lower_case(str);
    if (!valid_name(str))
    {
	input_to("get_name");
	catch_tell("Please give your name again: ", 1);
	return;
    }
    if (file_size("/syslog/banish/" + extract(str, 0, 0) + "/" + str) >= 0 ||
	file_size(LOGIN_APPLICATION_DIR + "/" + str + ".o") > 0)
    {
	catch_tell("That name is reserved.\nPlease give your name again: ", 1);
	input_to("get_name");
	return;
    }
    if (LANG_WNUM(str))
    {
	catch_tell("Numberwords are not allowed for playernames.\n" +
		   "Please give your name again: ", 1);
	input_to("get_name");
	return;
    }
    call_out("kick_idlers", 0);
    if (str == "guest")
    {
	name = "spirit";
	catch_tell("\nWelcome to '" + MUD_NAME + "'.\n\n", 1);
	start_player(0);
	return;
    }

  if (file_size((file = PLAYER_FILE(str)) + ".o") < 1)
    {
	if (_SYSTEMD->check_newplayer(query_ip_number()) == 2)
	{
	    catch_tell("You may not login new characters "+
		       "from your site!\n", 1);
	    destruct_object();
	    return;
	}
	catch_tell("\nDo you want to create a new character (y/n)? ", 1);
	input_to("certify_new_player", 0, str);
	return;
    }

    if (file_size(file + ".dead.o") > 0)
	file += ".dead";
    if (!restore_object(file))
    {
	catch_tell("Loading your character caused problems.\n" +
		   "Please contact the administration.\n", 1);
	destruct_object();
	return;
    }
    input_to("check_password", 1);
    catch_tell("\nWelcome to '" + MUD_NAME + "', " + capitalize(name)+
	       ".\nPlease give me your password: ", 1);
}

static nomask void
certify_new_player(string yesno, string str)
{
    if (yesno == "yes" || yesno == "y")
    {
	name = str;
	start_player(1);
	return;
    }
    catch_tell("\nThen, what should be your characters name: ", 1); 
    input_to("get_name");
}

/*
 * Function name: check_password
 * Description:   receive a password and check 
 * Arguments:     p - the password
 */
static nomask void
check_password(string p)
{
    if (!password || !strlen(password))
    {
	catch_tell("\n\nSorry, you have no password!\nPlease login as " +
		   "guest and mail to admin.\n", 1);
	destruct_object();
	return;
    }
    if (crypt(p, password) != password)
    {
	catch_tell("\nWrong password!\nPlease input your name again: ", 1);
	input_to("get_name");
	return;
    }
    start_player(0);
}

/*
 * Function name: enter_game
 * Description:   Swapsocket to player object and enter the game.
 * Arguments:	  ob - Player object.
 *                flag - true if linkdead
 */
static nomask void
enter_game(object ob, int flag)
{
    int i, level;
    
    exec(ob, this_object());/* Swap interactive object */

#ifdef _LOG_ENTER
    syslog("ENTER", BS(name + " from " + query_ip_number(ob), 76, 1));
#endif    
    
    notify_coders(ob, flag);
    set_this_player(ob);
    
    if (!flag)
	ob->start_player(name);
    else 
	ob->set_linkdead(0);
    if (!ob)
    {
	/* this could happen if starting location is not available */
	destruct_object();
	return;
    }
    level = _ACCESS_LEVEL(name);
    /* must be done after 'start_player' */
    if (file_time(LOGIN_FILE_NEWS) > (i = ob->query_prop(PLAYER_NEWS)) ||
	(level > 0 && file_time(LOGIN_CODER_NEWS) > i))
    {
	ob->catch_tell("\n *---------------------------------- News " +
		       "-----------------------------------*\n", 1);
	if (file_time(LOGIN_FILE_NEWS) > i)
	    tail(LOGIN_FILE_NEWS);
	if (level > 0 && file_time(LOGIN_CODER_NEWS) > i)
	    tail(LOGIN_CODER_NEWS);
	ob->catch_tell(" *------------------------------------------" +
		       "---------------------------------*\n", 1);
    }
    ob->add_prop(PLAYER_NEWS, time());
    
    _SYSTEMD->remove_player(ob);
    _SYSTEMD->add_player(ob);
    MAIL_MASTER->check_mail(name, ob); /* new mail? */

    if (level > 0)
    {
	if (level == _ADMIN &&
	    (i = sizeof(get_dir(LOGIN_APPLICATION_DIR + "/*.o"))) > 0)
	    ob->catch_tell("\nThere are " + i +
			   " player applications waiting.\n", 1);
	NEWS_MASTER->read_news(ob, 1);
    }
    _SYSTEMD->notify_login(ob, level);
    destruct_object();
}

/*
 * Function name: start_player
 * Description:   Initializes the player and prepares for game entry
 * Arguments:     i - 1 if new player, 0 else
 */
static nomask void
start_player(int i)
{
    object ob;

    if (name == "spirit")
    {
	ob = clone_object(RACE_FILE(name));
	name += "#" + (explode(object_name(ob),"#")[1]);
	ob->catch_tell("\nType 'help' to get some information.\n");
	enter_game(ob, 0);
	return;
    }
    /* See if we are already playing */
    if ((ob = find_player(name + ".dead")) ||
	((ob = find_player(name)) && race_name != "ghost"))
    {
	catch_tell("\n *** You were linkdead... "+
		   "Reconnecting... ***\n", 1);
	enter_game(ob, 1);
	return;
    }
    if (i)
    {
	LOGIN_GENERATE_PLAYER->generation(this_object(), name);
	return;
    }

   if (!race_name || file_size(RACE_FILE(race_name) + ".c") < 1)
    {
		catch_tell("\nHmm, this race is somehow lost.\n" +
			   "Please log in with 'guest' "+
			   "and contact the administration.\n", 1);
		destruct_object();
		return;
    }
    if (_ACCESS_LEVEL(name) > 0)
    {
			race_name = "ainu";
			ob = clone_object(RACE_FILE(race_name));
			ob->set_race(race_name);
    }
    else
	ob = clone_object(RACE_FILE(race_name));
    setuid(name, ob);
    seteuid(name, ob);
    enter_game(ob, 0);
}

/*
 * Function name: kick_idlers
 * Description:   get rid of idlers. Coders can stay longer 
 */
static nomask void
kick_idlers()
{
    int    i, lev;
    object *list;

    debug("manwe", "kick idler disabled");
    return;
    
    for (i=sizeof(list=users()); i--;)
    {
	lev = _ACCESS_LEVEL(geteuid(list[i]));
	if (query_idle(list[i]) > MAX_IDLE_TIME * (lev ? 10 : 1))
	{
	    list[i]->catch_tell("You have been idle too long. "+
				"You are logged out.\n", 1);
	    list[i]->command("quit");
	}
    }
}
