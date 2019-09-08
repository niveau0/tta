/* contains the miscellanous functions for a living */

#include <combat.h>
#include <config.h>
#include <effects.h>
#include <event.h>
#include <living.h>
#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <race.h>
#include <rr_types.h>
#include <skills.h>
#include <properties.h>
#include "prototypes.h"

int            fatigue;         /* How easily this lifeform is fatigued */
string         race_name;	/* The name of the race */
mapping        effects;	        /* poison and disease affecting this living */

private int fatigue_time,
            msg_flags;         

/*
 * Function name: extra_reset
 * Description:   set some default values, init arrays
 */
static nomask void 
extra_reset()
{
    fatigue_time = time();
}

/*
 * Function name: add_effect
 * Description:   set an effect. If the path is just an integer effect-id
 *                (see also effects.h) to store special effects, 'count' is
 *                not needed.
 * Arguments:     type - the effect type, e.g. POISON_EFFECT
 *                path - a file responsible for the effect or
 *                       integer code for effect reason/source <see effects.h>
 *                count - how long are we infected (effect specific)
 */
nomask void
add_effect(int type, mixed path, int count)
{
    object env, *r;
    
    if (!type || !path || !count)
	return;
    if (intp(path)) /* ignore count if path is specialeffect id */
	count = 1;
    if (!effects)
	effects = ([ type:([ path:count ]) ]);
    else if (!effects[type])
	effects[type] = ([ path:count ]);
    else 
	effects[type][path] = count;

    if (type == BLIND_EFFECT && (msg_flags & MSG_SEE))
	msg_flags -= MSG_SEE;
    else if (type == DEAF_EFFECT && (msg_flags & MSG_HEAR))
	msg_flags -= MSG_HEAR;
    if (!(env = E(this_object())))
	return;
    if (!(msg_flags & (MSG_HEAR | MSG_SEE)))
    {
	int i;
	r = map_indices(env->query_loaded_exit());
	remove_event(E_MSG, "?", env);
	for (i=sizeof(r); i--;)
	    remove_event(E_MSG, "?", r[i]);
    }
}

/*
 * Function name: remove_effect
 * Description:   remove an effect
 * Arguments:     type - the effect type, e.g. POISON_EFFECT
 *                path - file responsible for the effect, integer if special
 *                       effect
 */
nomask void
remove_effect(int type, mixed path)
{
    object env, *r;
    
    if (!type || !path)
	return;
    if (!effects || !effects[type])
	return;
    effects[type][path] = 0;
    if (!map_sizeof(effects[type]))
	effects[type] = 0;

    if (type == BLIND_EFFECT)
	msg_flags |= MSG_SEE;
    else if (type == DEAF_EFFECT)
	msg_flags |= MSG_HEAR;
    if (!(env = E(this_object())))
	return;
    if (msg_flags & (MSG_HEAR | MSG_SEE))
    {
	int i;
	r = map_indices(env->query_loaded_exit());
	add_event(E_MSG, "?", env);
	for (i=sizeof(r); i--;)
	    add_event(E_MSG, "?", r[i]);
    }
}

/*
 * Function name: query_effects
 * Description:   gives the effects
 * Returns:       mapping
 */
nomask mapping
query_effects()
{
    if (!effects)
	return ([ ]);
    return effects;
}

/*
 * Function name: init_msg_flags()
 * Description:    initialize message events and message flags
 */
nomask void
init_msg_flags()
{
    object env, *r;
    
    if (!effects || !effects[BLIND_EFFECT])
	msg_flags |= MSG_SEE;
    if (!effects || !effects[DEAF_EFFECT])
	msg_flags |= MSG_HEAR;
    if (!(env = E(this_object())))
	return;
    if (msg_flags & (MSG_HEAR | MSG_SEE))
    {
	int i;
	r = map_indices(env->query_loaded_exit());
	add_event(E_MSG, "?", env);
	for (i=sizeof(r); i--;)
	    add_event(E_MSG, "?", r[i]);
    }
}

/*
 * Functione name: query_msg_flags()
 * Description:    Get the currently set message-flags
 * Returns:        integer
 */
nomask int
query_msg_flags()
{
    return msg_flags;
}

/*
 * Function name:   set_race(string str)
 * Description:     Sets the race name of this living.
 * Arguments:       str - race string
 */
nomask void
set_race(string str)
{
    remove_name(race_name);
    race_name = str;
    add_name(race_name);
}

/*
 * Function name:   query_race()
 * Description:     Gives the race name of this living
 */
nomask string
query_race()
{
    return race_name;
}

/*
 * Function name:   query_npc()
 * Description:     Checks whether the living is a non-player character
 * Returns:         True if non-player character
 */
nomask int
query_npc()
{
    return !(function_object("save", this_object()) == PLAYER_BASE);
}

/*
 * Function name:   query_max_fatigue()
 * Description:     Calculates the maximum number of fatigue points that
 *                  the living can have.
 * Returns:         The maximum.
 */
nomask int
query_max_fatigue()
{
    if (query_npc())
	return query_prop(LIVE_NPC_MAX_FATIGUE);
    return query_stat(STAT_CO) + query_skill_bonus(SK_DISTANCE_RUN);
}

/*
 * Function name:   set_fatigue(int f)
 * Description:     Set the fatigue points of the living to a certain amount.
 * Arguments:       f - The amount to set.
 */
nomask void
set_fatigue(int f)
{
    if (f < 0)
	fatigue = 0;
    else if (f > query_max_fatigue())
	fatigue = query_max_fatigue();
    else
	fatigue = f;
}

/*
 * Function name:   add_fatigue(int f)
 * Description:     Add an amount of fatigue points to the current amount
 *                  of the living
 * Arguments:       f - the amount of change
 */
nomask void
add_fatigue(int f)
{
    set_fatigue(fatigue + f);
}

/*
 * Function name:   query_fatigue()
 * Description:     Gives the amount of fatigue points of a living
 * Returns:         The number of fatigue points
 */
nomask int
query_fatigue()
{
    int h;
    
    if (!fatigue_time)
    {
	fatigue_time = time();
	return fatigue;
    }
    /* add fatigue each 30 'mud'seconds */
    h = (time() - fatigue_time) * TIME_SCALE / 30;
    if (!h) return fatigue;
    fatigue_time += h * 30 / TIME_SCALE;
    set_fatigue(fatigue + h);
    return fatigue;
}

/*
 * Function name:   can_hear(object ob, int d)
 * Description:     checks if this object can hear another object
 * Arguments:       ob - object to hear
 *                  d - the distance something can be heard
 * Returns:         1 if hearing, else 0
 */   
nomask int
can_hear(object ob, int d)
{
    if (!ob || !ob->check_audible(d - query_prop(LIVE_DEAFNESS),
				  query_coordinates()))
	return 0;
    return 1;
}

/*
 * Function name:   can_see(object ob)
 * Description:     checks if this object can see another object
 * Arguments:       ob - object to see
 * Returns:         1 if visible, else 0
 */   
nomask int
can_see(object ob)
{
    if (!ob || !E(this_object()) || !(msg_flags & MSG_SEE) ||
	query_prop(LIVE_USING_SLA) ||
	!ob->check_visible((query_light() +
			    E(this_object())->query_light(query_light()) +
			    query_prop(LIVE_SEE_DARK)),
			   query_prop(LIVE_SEE_INVIS),
			   query_skill_bonus(SK_PERCEPTION),
			   query_coordinates(), E(this_object())))
	return 0;
    return 1;
}

/*
 * Function name: visible
 * Description:   removes not visible objects from object array
 * Arguments:     ob - the objects to check
 * Returns:       an array with visible objects
 */
nomask object *
visible(object *ob)
{
    int    i, sz, light, percep, see_invis, *co;
    object env, *vis;
    
    env = E(this_object());
    if (!(sz = sizeof(ob)) || !env || !(msg_flags & MSG_SEE) ||
	query_prop(LIVE_USING_SLA))
	return ({ });

    /* other items within rooms produce only 20% light for each other object
     * so the correct light value is:
     *   this livings own carried light +
     *   (room light - this livings own carried light) * 20 / 100
     */
    light     = query_light() + env->query_light(query_light()) +
		query_prop(LIVE_SEE_DARK);
    see_invis = query_prop(LIVE_SEE_INVIS);
    percep    = query_skill_bonus(SK_PERCEPTION);
    
    co  = query_coordinates();
    vis = allocate(sz);
    for (i=0; i<sz; i++)
    {
	if (!ob[i]->check_visible(light, see_invis, percep, co, env))
	    continue;
	vis[i] = ob[i];
    }
    return vis - ({ 0 });
}

/*
 * Function name: resistance_roll(int attack_type, int attack_level,
 *                                int modifier)
 * Description:   checks, if living succeeds to resist against the
 *                attacking forces
 * Arguments:     type - type of attack (e.g. one realm, poison, ...)
 *                attack_level - level of this given attack
 *                modifier - harder if negativ, positiv easier
 * Returns:       returns (roll - needed), a value greater 0 indicates
 *                a resistance of target. the returned value can be used to
 *                test if poison or something else takes not the full
 *                effect.
 */
nomask int
resistance_roll(int attack_type, int attack_level, int modifier)
{
    int   needs, resist;
    mixed race_data;

    needs = 50 - modifier;

    if (attack_level > 14)
	needs += 30 + attack_level;
    else if (attack_level >  9)
	needs += 15 + attack_level * 2;
    else if (attack_level >  4)
	needs += 5 + attack_level * 3;
    else
	needs += attack_level * 5;

    race_data = LEARN_OB->query_race_mod(query_race());
    
    switch (attack_type)
    {
      case RR_CHA:
	  resist = query_skill_rank(SK_RESIST_CHA);
	  needs -= (cb_query_bonus(CB_RESIST_CHA) +
		    query_stat_bonus(STAT_IN) + 
		    (race_data ? race_data[RRM_CHA] : 0));
	  break;
      case RR_ESS:
	  resist = query_skill_rank(SK_RESIST_ESS);
	  needs -= (cb_query_bonus(CB_RESIST_ESS) +
		    query_stat_bonus(STAT_EM) +
		    (race_data ? race_data[RRM_ESS] : 0));
	  break;
      case RR_MEN:
	  resist = query_skill_rank(SK_RESIST_MEN);
	  needs -= (cb_query_bonus(CB_RESIST_MEN) +
		    query_stat_bonus(STAT_PR) +
		    (race_data ? race_data[RRM_MEN] : 0));
	  break;
      case RR_ARC:
	  resist = (query_skill_rank(SK_RESIST_CHA) +
		    query_skill_rank(SK_RESIST_ESS) +
		    query_skill_rank(SK_RESIST_MEN)) / 3;
	  needs -= (cb_query_bonus(CB_RESIST_ESS) +
		    cb_query_bonus(CB_RESIST_MEN) +
		    cb_query_bonus(CB_RESIST_CHA) +
		    query_stat_bonus(STAT_PR) +
		    query_stat_bonus(STAT_IN) +
		    query_stat_bonus(STAT_EM) +
		    (race_data ? (race_data[RRM_ESS] +
				  race_data[RRM_MEN] +
				  race_data[RRM_CHA]) : 0)) / 3;
	  break;
      case RR_MEN | RR_CHA:
	  resist = (query_skill_rank(SK_RESIST_CHA) +
		    query_skill_rank(SK_RESIST_MEN)) / 2;
	  needs -= (cb_query_bonus(CB_RESIST_CHA) +
		    cb_query_bonus(CB_RESIST_MEN) +
		    query_stat_bonus(STAT_PR) +
		    query_stat_bonus(STAT_IN) +
		    (race_data ? (race_data[RRM_MEN] +
				  race_data[RRM_CHA]) : 0)) / 2;
	  break;
      case RR_MEN | RR_ESS:
	  resist = (query_skill_rank(SK_RESIST_ESS) +
		    query_skill_rank(SK_RESIST_MEN)) / 2;
	  needs -= (cb_query_bonus(CB_RESIST_ESS) +
		    cb_query_bonus(CB_RESIST_MEN) +
		    query_stat_bonus(STAT_PR) +
		    query_stat_bonus(STAT_EM) +
		    (race_data ? (race_data[RRM_ESS] +
				  race_data[RRM_MEN]) : 0)) / 2;
	  break;
      case RR_CHA | RR_ESS:
	  resist = (query_skill_rank(SK_RESIST_CHA) +
		    query_skill_rank(SK_RESIST_ESS)) / 2;
	  needs -= (cb_query_bonus(CB_RESIST_ESS) +
		    cb_query_bonus(CB_RESIST_CHA) +
		    query_stat_bonus(STAT_IN) +
		    query_stat_bonus(STAT_EM) +
		    (race_data ? (race_data[RRM_ESS] +
				  race_data[RRM_CHA]) : 0)) / 2;
	  break;
      case RR_POISON:
	  resist = query_skill_rank(SK_RESIST_POISON);
	  needs -= (cb_query_bonus(CB_RESIST_POISON) +
		    query_stat_bonus(STAT_CO) +
		    (race_data ? race_data[RRM_POISON] : 0));
	  break;
      case RR_DISEASE:
	  resist = query_skill_rank(SK_RESIST_DISEASE);
	  needs -= (cb_query_bonus(CB_RESIST_DISEASE) +
		    query_stat_bonus(STAT_CO) +
		    (race_data ? race_data[RRM_DISEASE] : 0));
	  break;
    }

    if (resist > 14)
	needs -= 30 + resist;
    else if (resist >  9)
	needs -= 15 + resist * 2;
    else if (resist >  4)
	needs -= 5 + resist * 3;
    else
	needs -= resist * 5;

    if (query_realm() & attack_type)
	needs -= 15;

    needs = roll_dice(-1) - needs;
    add_prop(LIVE_LAST_RR_RESULT, needs);
    
    return needs;
}
