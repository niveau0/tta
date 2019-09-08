#include <combat.h>
#include <event.h>
#include <effects.h>
#include <language.h>
#include <living.h>
#include <kernel.h>
#include <macros.h>
#include <mudtime.h>
#include <config.h>
#include <slots.h>
#include <state.h>
#include <properties.h>
#include "prototypes.h"

int             hp,
                base_hp,
		bloodloss;    /* total of bloodwounds */
mapping         bloodwounds;  /* each bloodwound */

private int     hp_time,
                max_hp;
private mapping team,         /* all team members, I'm leader */
                team_invited; /* Array of players invited to team */

/*
 * Function name: combat_reset
 * Description:   Reset the combat variables 
 */
static nomask void
combat_reset()
{
    team         = ([ ]);
    team_invited = ([ ]);
}

/*
 * Function name: team_invite
 * Description:   Invites a new member to my team. This does NOT join the
 *                member to my team. It only makes it possible for the
 *                player to join my team.
 * Arguments:	  ob -  The objectpointer to the invited member.
 */
nomask void
team_invite(object ob)
{
    team_invited[ob] = 1;
}

/*
 * Function name: team_remove_invite
 * Description:   Removes a team invitation
 * Arguments:	  ob - The objectpointer to the already invited member.
 */
nomask void
team_remove_invite(object ob)
{
    team_invited[ob] = 0;
}

/*
 * Function name: team_join
 * Description:   adds a member to a team
 * Arguments:	  ob - The objectpointer to the new member of my team
 *                flag - if true do not 'join' other members again 
 */
nomask void
team_join(object ob, int flag)
{
    int    i, sz;
    object *ind;
    
    if (!ob || !living(ob))
	return;
    team[ob] = 1;
    if (flag)
	return;
    
    ind = map_indices(team) - ({ this_object() });
    for (i=0, sz=sizeof(ind); i<sz; i++)
	ind[i]->team_join(ob, 1);
}

/*
 * Function name: team_leave
 * Description:   Removes this living as the leader of another
 * Arguments:	  ob - The objectpointer to the member leaving my team
 *                flag - if true do not 'leave' other members again 
 */
nomask void 
team_leave(object ob, int flag)
{
    int    i, sz;
    object *ind;
    
    if (!ob || !living(ob) || !team[ob])
	return;
    team[ob] = 0;
    if (flag)
	return;
    ind = map_indices(team) - ({ this_object() });
    for (i=0, sz=sizeof(ind); i<sz; i++)
	ind[i]->team_leave(ob, 1);
}

/*
 * Function name: query_invited
 * Description:   Give back an array of players who are invited
 * Returns:       array
 */
nomask object *
query_invited()
{
    return map_indices(team_invited);
}

/*
 * Function name: query_team
 * Description:   Gives an array with all team members
 * Returns:       array
 */
nomask object *
query_team()
{
    return map_indices(team);
}

/******************* bloodloss & unconscious ***************/ 

/*
 * Function name: bloodloss_event
 * Description:   controls bleeding
 */
static nomask void
bloodloss_event()
{
    int fs;
    
    if (!bloodloss)
	return;                               
    if (bloodloss - (fs = query_prop(LIVE_BLOODFLOW_STOP)) > 0)
    {
	catch_msg("You are bleeding!\n", this_object(), MSG_SEE);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " is bleeding.\n" }),
		     MSG_SEE, this_object());
	check_death(bloodloss - fs, 0);
	call_out("bloodloss_event", 10);
    }
    else
	call_out("bloodloss_event", 20);
}

/*
 * Function name: query_bloodwouds
 * Returns:       where we bleed
 */
nomask mapping
query_bloodwounds()
{
    if (!bloodwounds)
	return ([ ]);
    return bloodwounds;
}

/*
 * Function name: add_bloodloss
 * Description:   Set the amount how much bloodloss every beat
 * Arguments:     bl - The amount of bloodloss
 *                slot - which slot should bleed
 * Returns:       1 if success, 0 else
 */
nomask int
add_bloodloss(int bl, int slot)
{
    string  desc;
    mapping wounds;
    
    if (query_prop(LIVE_NEVER_BLEED))
	return 0;
    if (!bl)
	return 0;
    desc = query_slot_desc(slot);
    if (bl < 0)
    {
	if (!bloodwounds)
	    return 1;
	bl = -bl;
	if (bloodwounds[slot] > bl)
	{
	    bloodloss -= bl;
	    bloodwounds[slot] -= bl;
	}
	else if (bloodwounds[slot])
	{
	    bloodloss -= bl;
	    bloodwounds[slot] = 0;
	    if (!desc || query_wounds()[slot] == SLOT_LOST)
		return 1;
	    catch_msg("Your " + desc + " stops bleeding.\n", this_object(),
		      MSG_SEE);
	    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
			 ({ QCTNAME(this_object()), "'s " + desc +
				" stops bleeding.\n" }),
			 MSG_SEE, this_object());
	}
	return 1;
    }
    if (!bloodwounds)
	bloodwounds = ([ ]);
    if (!bloodwounds[slot] && desc)
    {
	catch_msg("Your " + desc + " starts to bleed.\n", this_object(),
		  MSG_SEE);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()),
			"'s " + desc + " starts to bleed.\n" }),
		     MSG_SEE, this_object());
    }
    if (!bloodloss)
	call_out("bloodloss_event", 2);
    DISEASE_TETANUS(this_object(), 40, slot);
    bloodwounds[slot] += bl;
    bloodloss += bl;
    return 1;
}

/*
 * Function name: query_bloodloss
 * Returns:       How strong we bleed
 */
nomask int
query_bloodloss()
{
    return bloodloss;
}

/*
 * Function name: stop_unconscious
 * Description:   awake from unconsciousness
 * Arguments:     force - optional,
 *                        if 1, no hp check 
 *                        if 2, no hp check and no wake up message
 */
nomask varargs void
stop_unconscious(int force)
{
    if (!(query_state() & UNCONSCIOUS))
	return;
    if (!force && query_hp() < 0)
    {
	call_out("stop_unconscious", 30);
	return;
    }
    remove_effect(BLIND_EFFECT, EFF_UNCONSCIOUS);
    remove_effect(DEAF_EFFECT, EFF_UNCONSCIOUS);
    delete_state(UNCONSCIOUS);
    remove_extra_desc(" (unconscious)");
    if (previous_object())
	remove_call_out("stop_unconscious");

    if (force != 2)
    {
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " wakes up.\n" }),
		     MSG_SEE, this_object());
	catch_tell("You wake up.\n");
    }
}

/*
 * Function name: add_unconscious
 * Description:   sets the unconscious time for a living
 * Arguments:     t - time how long to stay unconscious
 * Returns:       1 if success
 */
nomask int
add_unconscious(int t)
{
    int    i, remaining, *co;
    object env, *obj;
    
    if (t < 0 || query_prop(LIVE_NEVER_UNCONSCIOUS) || !living(this_object()))
	return 0;
    if (!(query_state() & UNCONSCIOUS))
    {
	add_effect(BLIND_EFFECT, EFF_UNCONSCIOUS, 1);
	add_effect(DEAF_EFFECT, EFF_UNCONSCIOUS, 1);
	add_state(UNCONSCIOUS);
	add_extra_desc(" (unconscious)");
	tell_objects(LISTEN((env = E(this_object()))) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " falls unconscious.\n" }),
		     MSG_SEE, this_object());
	catch_tell("You fall unconscious.\n");
	co = query_coordinates();
	obj = query_wield(SLT_ALL) + query_hold(SLT_ALL);
	for (i=sizeof(obj); i--;)
	{
	    obj[i]->reset_split();
	    obj[i]->set_coordinates(co);
	    obj[i]->move(env);
	}
    }
    remaining = remove_call_out("stop_unconscious");
    call_out("stop_unconscious", t + remaining + 1);
    return 1;
}

/***************************************************************************
 * death
 */

/*
 * Function name: notify_kill
 * Description:   This function is called when this object killed someone
 * Arguments:     liv - The living (object) that was killed.
 */
void
notify_kill(object liv)
{
    if (liv && E(this_object()) == E(liv))
	catch_msg(({ "You killed ", QTNAME(liv), ".\n" }), liv, MSG_SEE);
}

/*
 * Function name: make_corpse
 * Description:   redefineable corpse function
 * Returns:       0 - no special corpse at death
 *                1 - don't create normal corpse
 */
static int
make_corpse()
{
    return 0;
}
    
/*
 * Function name: resurrect
 * Description:   puts this living back to live
 */
nomask void
resurrect()
{
    add_prop(OBJ_NO_GET, -1);
    _SYSTEMD->resurrect_player(this_object());
    enable_commands(); /* mark object as alive */
    remove_effect(BLIND_EFFECT, EFF_DEATH);
    remove_effect(DEAF_EFFECT, EFF_DEATH);
    this_object()->start_call_out();
    remove_name("corpse");
    remove_name("corpse of " + query_race());
    if (hp < 1)
	hp = 1;
}

/*
 * Function name: do_die
 * Description:   Called when this living died.
 * Arguments:     killer - The enemy that caused our death.
 * Returns:       1 if success, else 0
 */
int
do_die(object killer)
{
    if (!living(this_object()))
	return 1;
    
    bloodloss = 0;	/* dead? stop bleeding! */
    bloodwounds = 0;
    
    stop_unconscious(2);
    delete_state(ALL_STATE);
    reset_extra_desc();
    add_effect(BLIND_EFFECT, EFF_DEATH, 1);
    add_effect(DEAF_EFFECT, EFF_DEATH, 1);

    if (!killer)
	killer = this_object();
    else
	killer->notify_kill(this_object());
    
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 ({ QCTNAME(this_object()), " died.\n" }), MSG_SEE,
		 this_object());
    
    disable_commands();
    break_possession(this_object(), this_object());

    if (!query_npc())
    {
#ifdef _LOG_KILLS	
	log("KILLS", BS(killer->query_name() + " (" + object_name(killer) +
	    ") killed " + query_name() + " at " + object_name(E(this_object())), 76, 1));
#endif
	catch_tell("You are dead. Your soul leaves your body.\n");
	_SYSTEMD->config_dead_soul(this_object());
	this_object()->stop_call_out();
    }
    add_prop(OBJ_NO_GET, 0);
    if (!make_corpse())
    {
	add_name("corpse");
	add_name("corpse of " + query_race());
    }
    call_event(E_DEATH, this_object(), killer);
    return 1;    
}

/*
 * Function name: decay
 * Description:   let this object (if dead) decay to dust
 */
nomask void
decay()
{
    int    i, *co;
    object env, *inv;
    
    if (living(this_object()))
	return;

    tell_objects(LISTEN((env = E(this_object()))) - ({ this_object() }),
		 ({ QCTNAME(this_object()), " decays to dust.\n" }),
		 MSG_SEE, this_object());
    
    co = env->query_coordinates();
    for (i=sizeof(inv=I(this_object(), 7)); i--;)
    {
	inv[i]->reset_split();
	inv[i]->set_coordinates(co);
	inv[i]->move(env);
    }
    remove_object();
}

/***************************************************************************
 * hit points
 */

/*
 * Function name: set_base_hp
 * Description:   sets the maximum hitpoints the living can have.
 * Arguments:     mhp - the maximum hitpoints
 */
nomask void
set_base_hp(int mhp)
{
    if (!query_npc() &&	(object_name(previous_object())) != LEARN_OB &&
	MASTER_OB(previous_object()) != CREATE_PLAYER)
	error("Illegal call to set_base_hp");
    base_hp = mhp;
    max_hp = 0; /* force init of max_hp */
}

/*
 * Function name: query_base_hp
 * Description:   the base hitpoints the living can have.
 * Returns:       integer
 */
nomask int
query_base_hp()
{
    return base_hp;
}

/*
 * Function name: query_max_hp
 * Description:   the maximum hitpoints the living can have.
 * Returns:       The maximum.
 */
nomask int
query_max_hp()
{
    if (!max_hp)
	max_hp = base_hp + base_hp * query_stat_bonus(STAT_CO) / 100;
    return max_hp;
}

/*
 * Function name: set_hp
 * Description:   Set the hit points of the living to a certain amount.
 * Arguments:     h: The amount to set.
 */
nomask void
set_hp(int h)
{
    if (h > query_max_hp())
	hp = query_max_hp();
    else
	hp = h;
}

/*
 * Function name: add_hp
 * Description:   Add an amount of hit points to the current amount
 *                of the living
 * Arguments:     h: the amount of change
 */
nomask void
add_hp(int h)
{
    set_hp(hp + h);
}

/*
 * Function name: query_hp
 * Description:   Gives the amount of hp points of a living
 *                hit points increase every 900 mudseconds. rest doubles
 *                the speed (450 mudsec), sleep triples (300 mudsec).
 * Returns:       The number of hp points
 */
nomask int
query_hp()
{
    int h, state;
    
    if (!hp_time)
    {
	hp_time = time();
	return hp;
    }
    if ((h = time() - hp_time) < 10 || !living(this_object()))
	return hp;
    if ((state = query_state()) & SLEEPING)
	/* add hp each 300 'mud'seconds */
	h = h * 3 * TIME_SCALE / 900;
    else if (state & RESTING)
	/* add hp each 450 'mud'seconds */
	h = h * 2 * TIME_SCALE / 900;
    else
	/* add hp each 900 'mud'seconds */
	h = h * TIME_SCALE / 900;
    if (!h)
	return hp;
    hp_time += h * 900 / TIME_SCALE;
    set_hp(hp + h);
    return hp;
}

/*
 * Function name: check_death
 * Description:   add damage and check if living should die, which occurs if
 *                the hitpoints are lower then the negative constitution value.
 * Arguments:     damage - the damage to add
 *                killer - the object that caused the damage
 * Returns:       0 if living is still alive
 *                1 if living is dead
 *                2 if living is unconscious
 */
nomask int
check_death(int damage, object killer)
{
    int h;

    add_hp(-damage);

    if ((h = query_hp()) > -1)
	return 0;
    if (!living(this_object()))
	return 1;
    if (h < -query_stat(STAT_CO))
	return do_die(killer);
    if (query_state() & UNCONSCIOUS)
	return 2;
    if (add_unconscious(30))
	return 2;
    return 0;
}

/*
 * Function name: E_got_hit
 * Description:   event: called if this living got a hit
 * Arguments:     from - which object did the hit
 *                ao - which object got hit
 *                damage - how much damage was inflicted
 *                slot - which slot got hit
 *                w - weapon object (0 if not hit by a weapon)
 */
nomask void
E_got_hit(object from, object ao, int damage, int slot, object w)
{
    object o;
	
    if ((query_state() & CONCENTRATE) &&
	damage > 0 && (hp - damage) < random(query_max_hp()))
    {
	catch_tell("You lost your concentration.\n");
	delete_state(CONCENTRATE);
	if (o = query_prop(CONCENTRATE_DATA))
	    o->lost_concentration(this_object());
    }
}
