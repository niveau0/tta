
inherit "/base/missile_weapon";


#include <combat.h>
#include <macros.h>
#include <material.h>
#include <properties.h>
#include <weapon.h>
#include <skills.h>
#include <event.h>

int *status;
/*
   0. -> activated or not, -> -1 damaged, 0 unactive, 1 active
   1.-> loaded or not -> 0 not loaded, 1 loaded
   */
int trap_ob, *range, disarm_difficulty, disarm_fumble_range;
private mapping ob_mod, at_mod;

object ammo, trap_activator;

void set_trap_ob(int ob);
void set_ammo(string ammo_path, int amount);
int reload();
int activate();
void set_range(int x, int y, int z);
int set_activator(string activator, object room);
void set_disarm_difficulty(int difficulty);
void set_disarm_fumble_range(int range);


void
create_trap()
{

}


void
create_weapon()
{
    set_name("hole");
    set_name("trap");
    set_short("hole");
    set_pshort("holes");
    set_long("A small hole in the wall. You wonder what it is for.\n");

    add_prop(OBJ_MATERIAL, M_STONE);
    add_prop(OBJ_NO_GET,1);
    add_prop(OBJ_NO_REMOVE_MSG,1);
    add_prop(OBJ_HIDE, 70);
    add_prop(OBJ_HIDE_PLACES, ({ "wall", "walls" }));
    status = ({ 0, 0 });
    set_trap_ob(75);
    set_ammo("/home/yavanna/moria/obj/trap/trap_bolt",1);
    add_lwvs(0, 4000, 200, 2);

    set_missile_data(SK_ATTACK_HEAVY_CROSSBOW,
		     ({ 2, 12, 15, 13, 17, 19, ({ PUNCTURE }) }),
		     75, 4);
		     
    
    set_disarm_difficulty(90);
    set_disarm_fumble_range(50);
 
    set_missile_type(MISSILE_BOLT);
    set_weapon_range(1200);
    set_value(0);
    set_range(3,3,120);
    add_event(E_MOVE, "enter_env", this_object());
    create_trap();
    reload();
    
}

int
set_activator(string activator, object room)
{
    object act;
    
    if(act = present("activator", room))
    {
	trap_activator = act;
	trap_activator->set_trap(this_object());
	return 1;
    }
    else
    {
	if(activator != "")
	{
	    trap_activator = clone_object(activator);
	    trap_activator->set_trap(this_object());
	    return 1;
	}
	return 0;
    }
    return 0;    
}

object
query_activator()
{
    return trap_activator;
}


int*
query_range()
{
    return range;
}

void
set_range(int x, int y, int z)
{
    range = ({ x, y, z });
}


string
long()
{
    if(!status[1])
	return ::long();
    return "There's a hole in the wall, nearly sealed.\nYou wonder what "+
	"it's good for.\n";
}

void
set_trap_ob(int ob)
{
    trap_ob = ob;
}

int
query_trap_ob()
{
    return trap_ob;
}

nomask void
enter_env(object ob, object dest, object old)
{
    if (!dest || ob != this_object())
	return;
    set_activator("",dest);
    activate();

/*    remove_event(E_MOVE,"enter_env", this_object());*/
}

void
set_ammo(string ammo_path, int amount)
{
    ammo = clone_object(ammo_path);
    ammo->set_heap_size(amount);
}

object
query_ammo()
{
    return ammo;
}


int
query_reloads()
{
    return ammo->query_heap_size();
}



int
reload()
{
    if(ammo && (!status[1]))
    {
	status[1] = 1;
	return 1;
    }
}

int
deactivate()
{
    if (status[0])
    {
	status[0] = 0;
	return 1;
    }
}


int
activate()
{
    if (status[1] && ammo)
    {
	if (!trap_activator || !(present("activator",here)))
	    return 0;
	trap_activator->activate();
	status[0] = 1;
	return 1;
    }
}

void
set_disarm_difficulty(int difficulty)
{
    disarm_difficulty = difficulty;
}

int
query_disarm_difficulty()
{
    return disarm_difficulty;
}

void
set_disarm_fumble_range(int range)
{
    disarm_fumble_range = range;
}

int
query_disarm_fumble_range()
{
    return disarm_fumble_range;
}

void
do_attack(object player)
{
    int fumble, ob_roll, *co_tar, damage, penalty, diff;
    object arrow, room;
    mixed hloc;
    
    if(status[0] && status[1] == 1)
    {
	status[1] = 0;
	reload();
	call_out("activate",(random(500)+3600));
	
	/* attack */
	if (!(fumble = query_missile_data()[A_FRANGE]))
	    fumble = 6;
	co_tar = player->query_coordinates();
	if ((ob_roll = roll_dice(-1)) <= fumble)
	{
	    tell_objects(LISTEN(E(player)),
			 ({"You hear snapping sound, but nothing happens.\n"}),
			 0,0);
			 
	    status[1] = -1;
	    return;
	}
	if (!(room = E(this_object())->query_leaving_room(co_tar)))
	    room = E(this_object());
	ammo->set_coordinates(co_tar);
	ammo->split_heap(1);
	ob_roll += query_trap_ob();
	ob_roll -= ob_roll * penalty / 100;
	diff = ob_roll - player->cb_query_missile_db();
	if (diff > 0)
	{
	    hloc = player->cb_random_hitloc();
	    tell_objects(LISTEN(E(player)) - ({ player}),
			 ({"You hear a hissing sound and then ",
			       QTNAME(player), " is hit by a ",
			       ammo->short(), ".\n" }));
	    player->catch_msg(({"You hear a hissing sound and then a ",
				    ammo->short(), " hits you.\n"}),
			      0, MSG_SEE);
	    player->check_death(DAMAGE_LIVING(this_object(),player, diff,
					      query_missile_data(),
					      hloc), this_object());
/*
	    if (diff > (hloc[HITLOC_THRES]+20))
	    {
		
	    */
	    /* gorfang */
/*		player->catch_tell("You are poisened with gorfang.\n"); */
/*
	    ammo->E_did_missile_hit(player,0);
		   }
		   */	
	    ammo->move(room, 0);
	    return;
	}
	tell_objects(LISTEN(E(player)) - ({ player }),
		    ({ "You hear a hissing sound and then a ",
			   ammo->short()," misses ",
			   QTNAME(player)," by some inches.\n" }),
		    MSG_SEE, player);
	player->catch_msg(({"You hear a hissing sound and then a ",
				ammo->short(), " misses you by some ",
				"inches.\n"}),
			  0, MSG_SEE);
	ammo->move(room, 0);     
	return;
    }
    tell_objects(LISTEN(E(player)),
		 "You hear a hissing sound, but nothing seems to happen.\n");
    ammo->move(room, 0);
}

int
disarm_cmd(object tp)
{
    int    skill, roll;

    if ((!status[0]) || (!ammo))
    {
	tp->catch_tell("Hmm, seems like this "+short()+" isn't dangerous.\n");
	return -1;
    }
    skill = tp->query_skill_bonus(SK_DISARM_TRAP);

    roll = roll_dice(-1) + skill;
    if (tp->query_race() == "dwarf")
	roll += 20;
    if (roll < disarm_difficulty)
    {
	tp->catch_tell("You failed.\n");
	if (roll <= query_disarm_fumble_range())
	{
	    tp->catch_tell("Argh, you set the trap off.\n");
	    tell_objects(LISTEN(E(this_object()))- ({ tp }),
			 ({ "Hmm, looks like ", QTNAME(tp), " sets off ",
				"a trap.\n" }), MSG_SEE, tp);
	    do_attack(tp);
	}
	return -1;
    }
    if (deactivate())
	tp->catch_tell("You disarmed the trap.\n");
    return 1;
}


varargs void
remove_object(object ob)
{
    if(query_activator())
	query_activator()->remove_trap(ob);
    remove_call_out("activate");
    ::remove_object(ob);
}

nomask varargs void
catch_msg(mixed msg, object from, int type, int dist, int no_prompt)
{}

nomask void
cb_add_boni(int *ids, mixed val)
{}



