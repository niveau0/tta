#include <access.h>
#include <config.h>
#include <macros.h>
#include <living.h>
#include <properties.h>
#include "../living/prototypes.h"

int           	login_time,	/* Last time logging in */
                active_time,	/* playing time since last improvement */
                prompt,         /* what prompt does the player want */
                birth;          /* the birthtime */
				
string	        password,       /* Password of the player */
		path,		/* Current directory path */
                email,		/* Email adress of the player */
                last_location,	/* Start location of player */
		login_from,	/* Host from last login */
                deity;          /* Name of the deity */
				
mixed	        auto_load;      /* Automatically loaded objects */
				
mapping         aliases,        /* aliases, $ for previous command */
                bits;           /* saved bits (quests etc.) */
		
/*
 * Function name: resolve_alias
 * Description:   check command for alias and expand
 * Arguments:     cmd - the command given by the player
 * Returns:       the expanded command if alias, else command not modified
 */
nomask string
resolve_alias(string cmd)
{
    if (!mappingp(aliases))
	return cmd;
    if (aliases[cmd])
	return aliases[cmd];
    return cmd;
}

/*
 * Function name: modifiy_alias
 * Description:   add/redefine/remove an alias
 * Arguments:     alias - the abbrevation
 *                expand - expandation for alias
 */
nomask void
modify_alias(string alias, string expand)
{
    if (geteuid(previous_object()) != geteuid())
	error("Illegal call to modify_alias");
    if (!mappingp(aliases))
	aliases = ([ alias:expand ]);
    else
	aliases[alias] = expand;
}

/*
 * Function name: query_aliases
 * Description:   gives all aliases
 * Returns:       alias mapping
 */
nomask mapping
query_aliases()
{
    return aliases;
}

/*
 * Function name: set_birth
 * Description:   sets the birthtime at first login
 * Arguments:     t - the time
 */
nomask void
set_birth(int t)
{
    if (MASTER_OB(previous_object()) != CREATE_PLAYER)
	error("Illegal call to set_birth");
    birth = t;
}
    
/*
 * Function name: query_birth
 * Description:   gives the birthtime
 * Returns:       integer time
 */
nomask int
query_birth()
{
    return birth;
}

/*
 * Function name: old_password, new_password, new_password2
 * Description:   change password
 */
static nomask void
old_password(string old)
{
    if (old != "" && crypt(old, password) != password)
    {
	catch_tell("\nWrong password.\n");
	return;
    }
    catch_tell("\nPlease input your new password: ", 1);
    input_to("new_password", 1);
}

static nomask void
new_password(string new)
{
    if (!new || strlen(new) < 6)
    {
	catch_tell("\nThe password must have a least 6 characters.\n");
	return;
    }
    catch_tell("\nPlease input your new password (again): ", 1);
    input_to("new_password2", 1, new);
}

static nomask void
new_password2(string new, string newpw)
{
    if (newpw != new)
    {
	catch_tell("\nYou changed.\n");
	return;
    }
    password = crypt(new);
    catch_tell("\nOk.\n");
    this_object()->save();
}

/*
 * Function name: set_password
 * Description:   Set the password of a player 
 * Arguments:     p - The new password string
 */
nomask void       
set_password(string p)
{
    if (MASTER_OB(previous_object()) != _LOGIN &&
	MASTER_OB(previous_object()) != CREATE_PLAYER)
	return;
    password = p;
}

/*
 * Function name: query_password
 * Description:   Gives back the password of a player
 * Returns:       The password string
 */
nomask string 
query_password()
{
    return password;
}

/*
 * Function name: set_auto_load
 * Description:   Sets the array with autoload strings.
 * Arguments:     a - An array with autoload strings.
 */
nomask void
set_auto_load(mixed a)
{
    auto_load = a;
}

/*
 * Function name: query_auto_load
 * Description:   Gives back the array of strings with objects to autoload
 *                when the player enters the game.
 * Returns:       An array of strings describing what objects to load.
 */
nomask mixed
query_auto_load()
{
    return auto_load;
}

/*
 * Function name: set_path
 * Description:   Sets the current path of a coder.
 * Arguments:     str - Pathstring
 */
nomask void
set_path(string str)
{
    path = str;
}

/*
 * Function name: query_path
 * Description:   Gives back the path of the current directory.
 * Returns:       The path string
 */
nomask string
query_path()
{
    if (path)
	return path;
    else if (_ACCESS_LEVEL(geteuid()))
	path = _SYSTEMD->query_coder_path(geteuid());
    return path;
}

/*
 * Function name: set_email
 * Description:   Sets the email address of a player
 * Arguments:     addr - the email address string
 */
nomask void
set_email(string addr)
{
    email = addr;
}

/*
 * Function name: query_email
 * Description:   Gives back the email address of a player.
 * Returns:       email address string
 */
nomask string
query_email()
{
    if (email)
	return email;
    return "none";
}

/*
 * Function name: set_last_location
 * Description:   set the last location of a player
 */
static nomask void
set_last_location()
{
    int *co;

    if (!E(this_object()))
	return;
    if (sizeof(co = query_coordinates()) == 3)
	last_location = (E(this_object())->query_room_file() + ":" +
			 co[0] + "," +  co[1] + "," + co[2]);
}

/*
 * Function name: query_last_location
 * Description:   Gives back the last location of a player
 * Returns:       The string with the filename of the startup-room.
 */
nomask string
query_last_location()
{
    return last_location;
}

/*
 * Function name: set_login_time
 * Description:   sets the time of the login
 */
static nomask void
set_login_time() 
{ 
    login_time = time(); 
}

/*
 * Function name:   query_login_time()
 * Description:     Gives back the login-time.
 * Returns:         The login-time.
 */
nomask int
query_login_time() 
{ 
    return login_time; 
}

/*
 * Function name: add_active_time
 * Description:   adds some time to the current active-playing-time
 */
static nomask void
add_active_time(int v) 
{ 
    active_time += v; 
}

/*
 * Function name: reset_active_time
 * Description:   sets the active-time to 0
 */
static nomask void
reset_active_time() 
{
    active_time = 0; 
}

/*
 * Function name:   query_active_time()
 * Description:     Gives back the currently active-play-time.
 * Returns:         integer
 */
nomask int
query_active_time() 
{ 
    return active_time; 
}

/*
 * Function name: set_login_from
 * Description:   Sets from which site the player is logged in.
 */
nomask void
set_login_from() 
{ 
    login_from = query_ip_number(this_object()); 
}

/*
 * Function name: query_login_from
 * Description:   shows from which site the player is logged in
 * Returns:       A string with the site name.
 */
nomask string
query_login_from() 
{ 
    return login_from; 
}

/*
 * Function name: set_bit
 * Description:   Set a given bit
 * Arguments:     group - the group to set the bit in
 *                bit - the bit to set
 */
nomask void
set_bit(int group, int bit)
{
    if (!mappingp(bits))
	bits = ([ ]);
    bits[group] |= (1 << bit);
}

/*
 * Function name: clear_bit
 * Description:   Clear a given bit
 * Arguments:     group - the group to set the bit in
 *                bit - the bit to clear
 * Returns:       1 if the bit was successfully cleared, 0 otherwise.
 */
nomask int
clear_bit(int group, int bit)
{
    if (_ACCESS_LEVEL(geteuid(previous_object())) < _CREATOR)
	return 0;
    if (!mappingp(bits))
	return 1;
    bits[group] -= (1 << bit);
    return 1;
}

/*
 * Function name: test_bit
 * Description:   Test a given bit in a given group for a given domain. 
 * Arguments:     group - which bit group
 *                bit - which bit to test
 * Returns:       1 if the bit was set, 0 if unset.
 */
nomask int
test_bit(int group, int bit)
{
    if (!mappingp(bits))
	return 0;
    if (bits[group] & (1 << bit))
	return 1;
    return 0;
}

/*
 * Function name: query_bits
 * Description:   Gives back the bits of a player
 * Returns:       the bit integer
 */
nomask mapping
query_bits()
{
    return bits;
}

/*
 * Function name: set_deity
 * Description:   What deity does the player worship
 * Arguments:     d - the name of the deity
 */
nomask string
set_deity(string d)
{
    if (!d)
	return "";
    deity = lower_case(d);
    return deity;
}

/*
 * Function name: query_deity
 * Description:   give the deity the player worships
 * Returns:       string
 */
nomask string
query_deity()
{
   return (deity ? deity : "");
}

/*
 * Function name: set_prompt
 * Description:   configure prompt output
 * Arguments:     flags - what should be prompted
 */
nomask void
set_prompt(int flags)
{
    prompt = flags;
}

/*
 * Function name: query_prompt
 * Description:   gives the wanted prompt 
 * Returns:       string
 */
nomask string 
query_prompt()
{
    int    i, max;
    string p;
    
    if (!prompt)
	return "> ";
    p = "";
    if (prompt & 8)
	p += query_path();
    if (prompt & 4)
    {
	if ((max = query_max_hp()) < (i = query_hp()) || max == 0)
	    i = 7;
	else
	{
	    i = i * 7 / max;
	    if (i < 0) i = 0;
	}
	switch (i)
	{
	  case 0:
	    p += " <Hp: near dying>"; break;
	  case 1:
	    p += " <Hp: awful>"; break;
	  case 2:
	    p += " <Hp: bad>"; break;
	  case 3:
	    p += " <Hp: wounded>"; break;
	  case 5:
	    p += " <Hp: hurt>"; break;
	  case 6:
	    p += " <Hp: scratches>"; break;
	  default:
	    p += " <Hp: full>"; break;
	}
    }
    if (prompt & 2)
    {
	if ((max = query_max_mana()) < (i = query_mana()) || max == 0)
	    i = 5;
	else
	{
	    i = i * 5 / max;
	    if (i < 0) i = 0;
	}
	switch (i)
	{
	  case 0:
	    p += " <Mp: exhausted>"; break;
	  case 1:
	    p += " <Mp: awful>"; break;
	  case 2:
	    p += " <Mp: bad>"; break;
	  case 3:
	    p += " <Mp: degraded>"; break;
	  case 4:
	    p += " <Mp: used>"; break;
	  default:
	    p += " <Mp: full>"; break;
	}
    }
    return p + "\n> ";
}