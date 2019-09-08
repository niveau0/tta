#include <access.h>
#include <config.h>
#include <kernel.h>
#include <net.h>
#include <status.h>
#include <trace.h>

#define SAVEFILE "/kernel/data/SYSTEMDATA" /* File to save object in */

#include "systemd/admin.c"
#include "systemd/call_out.c"
#include "systemd/shutdown.c"
#include "systemd/siteban.c"
#include "systemd/statistics.c"

nomask int exist_player(string name);
nomask object finger_player(string pl_name);

static object global_obj; /* finger-command object */

static nomask void
create()
{
    int    i;
    object *u;
    if (!restore_object(SAVEFILE))
    {
	catch(error(SAVEFILE + " nonexistant. Using defaults"));
	init_admin();
    }
    else /* start callout only if keepersave exists, maybe r/w bugs occured */
	call_out("check_reboot", 1);  
    
    /* must be done after restoring master data */
    init_statistics();

    for (i=sizeof(u = users()); i--;)
	add_player(u[i]);
    call_out("check_coders", 1);
}

/*
 * Function name: check_coders
 * Description:   has every coder in coders mapping still a playerfile? if
 *                not, demote him/her
 */
static nomask void
check_coders()
{
    int    i, sz;
    string path, *ainu;

    rlimits(100; 10000000)
    {
	ainu = query_coder_list(-1);
	for (i=0, sz=sizeof(ainu); i<sz; i++)
	{
	    if (!exist_player(ainu[i]))
		do_chcoder_lev(ainu[i], 0);
	    else 
	    {
		if (file_size(path = query_coder_path(ainu[i])) != -2)
		    make_dir(path);
		/* uncomment next line and reload master to update
		 * ALL access files
		 */
		/* update_access(({ ainu[i] })); */ 
	    }
	}
    }	
}

/*
 * Function name: check_reboot
 * Description:   checks if it's time to reboot, saves the master data
 */
static nomask void
check_reboot()
{
    int hour;
    
    remove_file(SAVEFILE + ".bak");
    rename_file(SAVEFILE + ".o", SAVEFILE + ".bak");
    save_object(SAVEFILE);

    call_out("check_reboot", 60*10);

    write_logincount();
    
    if (REGULAR_REBOOT < 0)
	return;
    
    sscanf(ctime(time())[11..12], "%d", hour);
    if (hour == REGULAR_REBOOT &&
	time() - object_time(this_object()) > 3600)
	shutdown_game(this_object(), "10", "Regular reboot");
}

/*
 * Function name: exist_player
 * Description:   check if a player exists
 * Arguments:     name - the name of the player to check
 * Returns:       1 if player exists
 */
nomask int
exist_player(string name)
{
    if (!name)
	return 0;
    return (file_size(PLAYER_FILE(name) + ".o") > 0);
}

/*
 * Function name: resurrect_player
 * Description:   this is called if someone is dead but should live again
 */
nomask void
resurrect_player(object who)
{
    string name;
    object soul;
    
    if (who != previous_object() || who->query_npc())
	return;

    name = who->query_name();
    if (soul = find_player(name + ".dead"))
    {
	soul->catch_tell("A strong suction drags you back into your body.\n");
	exec(who, soul);
	remove_object(soul);
	remove_player(soul);
    }
    if (file_size(PLAYER_FILE(name) + ".dead.o") > 0)
	remove_file(PLAYER_FILE(name) + ".dead.o");
    add_player(who);
}

/*
 * Function name: config_dead_soul
 * Description:   this is called if someone dies. Cares for reloading corpse
 *                and a soul object
 * Arguments:     who - who died
 */
nomask void
config_dead_soul(object who)
{
    object soul;
    
    if (!who || who->query_npc())
	error("Illegal call to config_dead_soul");
    
    soul = clone_object(DEAD_SOUL);
    exec(soul, who); /* must be done before config_soul */
    soul->set_name(who->query_name());
    soul->config_soul(who);
    soul->save();
    seteuid(geteuid(who), soul);
    remove_player(who);
    add_player(soul);
}

/*
 * Function name:   finger_player(string _pl_name)
 * Description:     Returns a player object restored with the values from
 *		    the players save file.
 * Arguments:	    pl_name - Name of a player
 * Returns:         The restored player object, 0 if not existing
 */
nomask object
finger_player(string pl_name)
{
    int    ret, lev;
    object ob;

    if (!exist_player(pl_name))
	return 0;

    if (!global_obj)
        global_obj = clone_object(PLAYER_BASE);
    ob = global_obj;

    ob->remove_name(ob->query_names());
    ob->remove_pname(ob->query_pnames());
    ob->remove_adj(ob->query_adjs());
    
    if (catch(ob->load(pl_name)))
	return 0;
    
    /*if (lev = _Q_coder_level(pl_name))
	ob->set_coder_level(lev);*/
    
    return ob;
}

nomask string
query_default_start_location()
{
    return DEFAULT_START;
}

nomask varargs void
remove_object(object ob)
{
    if (global_obj)
	::remove_object(global_obj);
    ::remove_object(ob);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
