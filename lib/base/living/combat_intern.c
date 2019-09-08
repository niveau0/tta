/* combat related functions */

#include <combat.h>
#include <config.h>
#include <event.h>
#include <living.h>
#include <macros.h>
#include <properties.h>
#include <slots.h>
#include <state.h>
#include <skills.h>
#include <weapon.h>
#include "prototypes.h"

private int     totalch,
                parry_skill;
private object  ao,
                parry_weapon,
		*shields,
		*enemies;
private mapping attacks,
                hitlocs,
		cb_boni;
		 
/*
 * Function name: cb_intern_reset
 * Description:   initialize variables
 */
static nomask void
cb_intern_reset()
{
    enemies = ({ });
    shields = ({ });
    attacks = ([ ]);
    hitlocs = ([ ]);
    cb_boni = ([ ]);
}

/*
 * Function name: cb_add_boni
 * Description:   add combat boni and mali
 * Arguments:     ids - integer ids , see <combat.h>
 */
nomask void
cb_add_boni(int *ids, mixed val)
{
    int i, sz;

    if (!(sz = sizeof(ids)))
	error("Bad argument 1 for function cb_add_boni");
    if (sizeof(val) != sz)
	error("Bad argument 2 for function cb_add_boni");
    for (i=0; i<sz; i++)
    {
	if (!mappingp(val[i]))
	    error("Bad argument 2 for function cb_add_boni");
	if (!mappingp(cb_boni[ids[i]]))
	    cb_boni[ids[i]] = val[i];
	else
	    cb_boni[ids[i]] += val[i];
    }
}

/*
 * Function name: cb_remove_boni
 * Description:   remove combat boni and mali
 * Arguments:     ids - array of bonus ids to remove, see <combat.h>
 *                val - array of indices to remove
 */
nomask varargs void
cb_remove_boni(int *ids, mixed val)
{
    int   i, sz;

    if (!(sz = sizeof(ids)))
	error("Bad argument 1 for function cb_remove_boni");
    if (sizeof(val) != sz)
	error("Wrong size of 2. argument to cb_add_boni");
    for (i=0; i<sz; i++)
	if (mappingp(cb_boni[ids[i]]))
	    cb_boni[ids[i]][val[i]] = 0;
}

/*
 * Function name: cb_query_bonus
 * Description:   gives a combat bonus, removes timed out boni
 * Arguments:     id - which bonus to query
 * Returns:       integer bonus
 */
nomask int
cb_query_bonus(int id)
{
    int     i, t, sum;
    mapping map;
    mixed   ind;

    if (!(map = cb_boni[id]))
	return 0;
    t   = time();
    ind = map_indices(map);
    for (i=sizeof(ind); i--;)
    {
	if (!objectp(ind[i]) && t > ind[i])
	    map[ind[i]] = 0;
	else
	    sum += map[ind[i]];
    }
    return sum;
}

/*
 * Function name: cb_bonus_map
 * Description:   gives the whole bonus mapping
 * Returns:       bonus map
 */
nomask mapping
cb_bonus_map()
{
    return cb_boni;
}

/*
 * Function name: cb_stop_fight
 * Description:   stop fight against given enemies
 * Arguments:     ob - the enemies
 */
nomask void
cb_stop_fight(mixed ob)
{
    if (!pointerp(ob))
    {
	enemies -= ({ ob });
	if (ao == ob)
	    ao = 0;
    }
    else
    {	
	enemies -= ob;
	if (IN_ARRAY(ao, ob))
	    ao = 0;
    }
}

/*
 * Function name: cb_add_attack
 * Description:   add an unarmed attack
 * Arguments:     slot - the slot that should be attack slot
 *                data - ({ sk_num - the skill for the attack,
 *                          at_modifier array (see <combat.h>),
 *                          base ob modifier
 *                       })
 */
static nomask void
cb_add_attack(int slot, mixed data)
{
    mixed am;
    
    if (sizeof(data) != 3 || sizeof(am = data[1]) != 7)
	error("Bad argument 2 for function cb_add_attack");
    if (!query_slot_desc(slot) || query_wounds()[slot])
	return;
    attacks[slot] = ([
	A_SKILL     : data[0],
	A_OBMOD     : data[2],
	A_DMOD      : am[0], 
	A_SKIN      : am[1], 
	A_S_LEATHER : am[2], 
	A_R_LEATHER : am[3], 
	A_CHAIN     : am[4], 
	A_PLATE     : am[5], 
	A_CRIT      : am[6], 
	]);
}

/*
 * Function name: cb_remove_attack
 * Description:   remove an unarmed attack
 * Arguments:     slot - the slot that should be removed
 */
static nomask void
cb_remove_attack(int slot)
{
    attacks[slot] = 0;
}

/*
 * Function name: cb_query_attack
 * Description:   gives the attack mapping for a slot
 * Arguments:     slot - the slot to query
 */
nomask mapping
cb_query_attack(int slot)
{
    return attacks[slot];
}

/*
 * Function name: cb_add_hitloc
 * Description:   adds a hitlocation
 * Arguments:     slot - the slot that can be hit
 *                data - ({ at - armor type
 *                          hc - chance how often this hitloc will be hit })
 */
static nomask void
cb_add_hitloc(int slot, int *data)
{
    if (sizeof(data) != 2)
	error("Bad argument 2 for function cb_add_hitloc");
    if (data[1] < 1)
	error("Hitchance too low");
    if (!query_slot_desc(slot) || query_wounds()[slot] == SLOT_LOST)
	return;
    hitlocs[slot]  = data;
    totalch       += data[1];
}

/*
 * Function name: cb_remove_hitloc
 * Description:   removes a hitloc from the hitlocs map
 * Arguments:     slot - location to remove
 */
static nomask void
cb_remove_hitloc(int slot)
{
    if (hitlocs[slot])
    {
	totalch       -= hitlocs[slot][1];
	hitlocs[slot]  = 0;
    }
}

/*
 * Function name: cb_query_hitloc
 * Description:   test if a given hitloc exists
 * Arguments:     slot = which location to query
 * Returns:       array, ({ at, hc }) of hitloc if existing, else 0
 */
nomask int *
cb_query_hitloc(int slot)
{
    return hitlocs[slot];
}

/*
 * Function name: cb_random_hitloc
 * Description:   returns a random hitlocation
 * Returns:       random hitloc - ({ slot, description, at, hc })
 */
nomask mixed
cb_random_hitloc()
{
    int   i, c, rnd, curr, *hlocs;
    
    hlocs = map_indices(hitlocs);
    rnd   = random(totalch);
    for (c=0, i=sizeof(hlocs); i--;)
    {
	c += hitlocs[curr = hlocs[i]][1];
	if (rnd < c)
	    return ({ curr, query_slot_desc(curr) }) + hitlocs[curr];
    }
    return 0; /* no hitloc found */
}

/*
 * Function name: cb_add_enemy
 * Description:   adds an object to the enemies
 * Arguments:     e - the new enemy/ies
 */
nomask void
cb_add_enemy(mixed e)
{
    if (!e)
	return;
    
    if (!pointerp(e))
	enemies |= ({ e });
    else
	enemies |= e;
}

/*
 * Function name: cb_query_enemies
 * Description:   return the enemies
 * Returns:       array of enemies
 */
nomask object *
cb_query_enemies()
{
    return enemies;
}

/*
 * Function name: cb_query_attacker
 * Description:   gives the current attack object
 * Returns:       object if fighting, else 0
 */
nomask object
cb_query_attacker()
{
    return ao;
}

/*
 * Function name: cb_no_melee_attack
 * Description:   determine if this living is able to attack 'ob' in
 *                melee combat
 * Arguments:     ob - who to attack
 * Returns:       1 if melee attack is not possible
 */
nomask int
cb_no_melee_attack(object ob)
{
    if (ob)
    {
	if (!living(ob)) /* remove killed enemies */
	{
	    enemies -= ({ ob });
	    return 1;
	}
	if (!direct_approach(this_object(), ob))
	    return 1;
    }
    return 0;
}

/*
 * Function name: cb_attack_object
 * Description:   Attack someone
 * Arguments:     ob - The object of the attack
 */
nomask void
cb_attack_object(object ob)
{
    if (!ob || ao == ob || ob == this_object())
	return;

    enemies -= ({ 0 });
    if (!IN_ARRAY(enemies, ob))
    {
	call_event(E_ATTACK, this_object(), ob);
	enemies += ({ ob });
    }    
    if (!ao)
    {
	call_out("cb_combat_round", 1);
	add_state(COMBAT);
	add_prop(LIVE_LOST_ROUNDS, 0);
    }
    
    ao = ob; /* switch attack to victim */

    /* no hide in combat */
    delete_state(HIDING);
    add_prop(OBJ_HIDE, 0);
    add_prop(OBJ_HIDE_PLACES, 0);
    ob->cb_attacked_by(this_object());
}

/*
 * Function name: cb_attacked_by
 * Description:   This is called when this objects gets attacked 
 * Arguments:	  ob - the attacker
 */
nomask void
cb_attacked_by(object ob)
{
    if (!ob || ao == ob)
	return;
    
    enemies -= ({ 0 });
    if (!IN_ARRAY(enemies, ob))
    {
	call_event(E_ATTACKED_BY, this_object(), ob);
	enemies += ({ ob });
    }
    if (ao || query_prop(LIVE_NO_AUTOATTACK))
	return;
    
    ao = ob;
    call_out("cb_combat_round", 2);
    add_prop(LIVE_LOST_ROUNDS, 0);
}

/*
 * Function name: cb_init_fight
 * Description:   initalizes fight while moving
 */
nomask void
cb_init_fight()
{
    int    i;
    object to;

    if (ao || !sizeof(enemies))
	return;
    if (!query_npc() && query_prop(LIVE_NO_AUTOATTACK))
	return;
    enemies -= ({ 0 });
    for (i=0; i<sizeof(enemies); i++)
	/* enemies array may change size in loop */
    {
	if (!cb_no_melee_attack(enemies[i]))
	{
	    cb_attack_object(enemies[i]);
	    if (!ao)
		return;

	    to = this_object();
	    tell_objects(LISTEN(E(this_object())) - ({ to, ao }),
			 ({ QCTNAME(to), " attacks ", QTNAME(ao), ".\n" }),
			 MSG_SEE, to);
	    ao->catch_msg(({ QCTNAME(to), " attacks you!\n" }), to,
			  MSG_HEAR | MSG_SEE, 1000);
	    catch_msg(({ "You attack ", QTNAME(ao), ".\n" }), 0, 0);
	    return;
	}
    }
}

/* 
 * Function name: tell_watcher
 * Description:   Send the string from the fight to people that want see
 * Arguments:	  The string to send
 */
static nomask void
tell_watcher(mixed str)
{
    int    i, sz;
    object *ob;
    
    ob = LISTEN(E(this_object())) - ({ ao, this_object() });
    for (i=0, sz=sizeof(ob); i<sz; i++)
	if (ob[i]->query_prop(LIVE_SEE_FIGHT))
	    ob[i]->catch_msg(str, this_object(), MSG_SEE);
}

/*
 * Function name: cb_message
 * Description:   prints the combat messages and deals with the damage
 * Arguments:     attack - the slot or weapon that made the attack
 *                aohloc - the hitlocation
 *                         ({ slot, at, ac, th, hc, description })
 *                diff - difference between ob and db
 */
private void
cb_message(mixed attack, mixed aohloc, int diff)
{
    int     i, damage;
    mixed   qctname, qtname, with;
    object  *armor;
    mapping at_data;
    
    if (objectp(attack))
	with = ({ attack, 0, attack->short(), "something" });
    else
	with = ({ this_object(), 0, query_slot_desc(attack), "something" });
    
    qctname = QCTNAME(this_object());
    qtname  = QTNAME(ao);

    if (objectp(attack))
	at_data = attack->query_data();
    else
	at_data = attacks[attack];
    diff += at_data[A_OBMOD]; /* attack ob-modifier */
    
    if (diff < 0)
    {
	/* hit was a miss */
	
	catch_msg(({ "You aim at ", qtname, " with your ", with,
		     ", but you miss.\n" }), this_object(), MSG_SEE);
	ao->catch_msg(({ qctname, " aims at you with " +
			     query_possessive() + " ", with,
			 ", but misses.\n" }), this_object(), MSG_SEE);
	tell_watcher(({ qctname, " aims at ", qtname,
			" with " + query_possessive() + " ", with,
			", but misses.\n" }));
	return;
    }

    catch_msg(({ "You hit ", qtname, " with your ", with, ".\n" }),
	      this_object(), MSG_SEE);
    ao->catch_msg(({ qctname, " hits you with " + query_possessive() + " ",
		     with, ".\n" }), this_object(), MSG_SEE);
    tell_watcher(({ qctname, " hits ", qtname, " with " + query_possessive() +
			" ", with, ".\n" }));
    damage = DAMAGE_LIVING(this_object(), ao, diff, at_data, aohloc);

    call_event(E_DID_HIT, this_object(), ao, damage, aohloc[HITLOC_SLOT],
	       (objectp(attack) ? attack : 0));
    ao->call_event(E_GOT_HIT, this_object(), ao, damage,
		   aohloc[HITLOC_SLOT], (objectp(attack) ? attack : 0));

    armor = ao->query_worn(aohloc[HITLOC_SLOT]);
    for (i=sizeof(armor); i--;)
	armor[i]->call_event(E_GOT_HIT, this_object(), ao, damage,
			     aohloc[HITLOC_SLOT],
			     (objectp(attack) ? attack : 0));
    
    if (ao) /* enemy object still around? */
    {
	if (damage < 1)
	{
	    ao->catch_msg("Lucky, you received no damage.\n", 0, MSG_SEE, 0);
	    return;
	}
	ao->check_death(damage, this_object());
    }
}

/*
 * Function name: cb_query_quickness
 * Description:   compute combat quickness in seconds (additionally fatigue
 *                slows down)
 * Returns:       quickness, integer
 */
nomask int
cb_query_quickness()
{
    int quick;
    
    quick = query_max_fatigue() * 10 / (query_fatigue() + 1) -
	    (query_stat_race_bonus(STAT_QU) + query_stat(STAT_QU) +
	     random(100) - weight_encumbrance()) / 10;
    if (quick < 2)
	return 2;
    return quick;
}

/*
 * Function name: cb_add_shield
 * Description:   adds a wielded shield
 * Arguments:     s - the shield
 */
nomask void
cb_add_shield(object s)
{
    if (!s)
	return;
    shields |= ({ s });
}

/*
 * Function name: cb_remove_shield
 * Description:   remove a shield
 * Arguments:     s - the shield
 */
nomask void
cb_remove_shield(object s)
{
    if (!s)
	return;
    shields -= ({ s, 0 });
}

/*
 * Function name: cb_set_parry_skill
 * Description:   set the skill to parry with if fighting unarmed
 * Arguments:     sk - the skill number
 */
nomask void
cb_set_parry_skill(int sk)
{
    parry_skill = sk;
}

/*
 * Function name: cb_query_parry_skill
 * Description:   give the parry skill used if fighting unarmed
 * Returns:       integer
 */
nomask int
cb_query_parry_skill()
{
    return parry_skill;
}

/*
 * Function name: cb_set_parry_weapon
 * Description:   chose a weapon to parry with, must be wielded
 * Arguments:     ob - the weapon
 */
nomask void
cb_set_parry_weapon(object ob)
{
    if (!ob || ob->query_wielder() != this_object() ||
	ob->query_prop(IS_SHIELD))
	return;
    parry_weapon = ob;
}
    
/*
 * Function name: cb_query_parry_weapon
 * Description:   gives current parry weapon, chooses new parry weapon
 *                if none set
 * Returns:       object or 0
 */
nomask object
cb_query_parry_weapon()
{
    int    i;
    object *w;
    
    if (parry_weapon && parry_weapon->query_wielder() == this_object())
	return parry_weapon;
    if (!(i = sizeof(w = query_wield(SLT_ALL))))
	return 0;
    while (i--)
	if (!w[i]->query_prop(IS_SHIELD))
	{
	    parry_weapon = w[i];
	    return parry_weapon;
	}
    return 0;
}

/*
 * Function name: cb_armor_penalty
 * Description:   check if some heavy armor is worn, db malus
 * Returns:       penalty
 */
nomask int
cb_armor_penalty()
{
    int    i, v, skill, penalty;
    object to, *worn;

    to = this_object();
    for (i=sizeof(worn = query_worn(SLT_FIRST(SLT_CHEST))); i--;)
    {
	switch (worn[i]->query_at())
	{
	  case A_S_LEATHER: skill = SK_SOFT_LEATHER; break;
	  case A_R_LEATHER: skill = SK_RIGID_LEATHER; break;
	  case A_CHAIN:     skill = SK_CHAIN; break;
	  case A_PLATE:	    skill = SK_PLATE; break;
	}
	if (!random(10))
	    to->skill_check(skill, 0, -1);
	if ((v = random(100 - query_skill_bonus(skill))) > 0)
	    penalty += v;
	break;
    }
    return penalty;
}

/*
 * Function name: cb_ob_db_mod
 * Description:   gives ob db modifications
 * Arguments:     flag - if true, do weapon malus check
 * Returns:       integer
 */
private int
cb_ob_db_mod(int flag)
{
    int mod, malus;
    
    if (!ao || can_see(ao))
	mod = 100;
    else if (query_prop(LIVE_USING_SLA))
    {
	if ((mod = query_skill_bonus(SK_SLA)) > 100)
	    mod = 100;
    }
    else
	mod = 5;
    
    if (!flag)
	return mod;
    
    if (sizeof(query_wield(SLT_ALL)) - sizeof(shields) > 1)
    {
	/* malus if using two weapons (shields don't count) */
	if ((malus = 90 - query_skill_bonus(SK_TWO_WEAPONS)) < 5)
	    malus = 5;
	mod -= malus;
    }
    return mod;
}

/*
 * Function name: cb_query_melee_ob
 * Description:   gives the offensive bonus for a given skill
 *                handles also blindness and spell effects,
 * Arguments:     skill - the skill to query ob for
 *                weap - weapon object, used to query for (magic) bonus
 * Returns:       integer ob
 */
nomask int
cb_query_melee_ob(int skill, object weap)
{
    int mod;

    mod = cb_query_bonus(CB_OB_BONUS);
    if (weap)
    {
	mod += cb_ob_db_mod(1);
	return (weap->query_bonus() + (query_skill_bonus(skill) * mod *
				       query_prop(LIVE_FIGHT_MOOD) / 10000));
    }
    mod += cb_ob_db_mod(0);
    return (query_skill_bonus(skill) * mod *
	    query_prop(LIVE_FIGHT_MOOD) / 10000);
}

/*
 * Function name: cb_query_melee_db
 * Description:   gives the defensive bonus
 * Arguments:     ob - attacking object, if not the current opponent,
 *                     no parry bonus
 * Returns:       integer db
 */
nomask int
cb_query_melee_db(object ob)
{
    int    db, mod, mood;
    object weap;

    db = (query_stat_bonus(STAT_QU) - cb_armor_penalty() +
	  query_skill_bonus(SK_ADRENAL_DEFENSE) + sizeof(shields) * 25);
    
    mod = cb_query_bonus(CB_DB_BONUS);
    
    if ((ob && ob != ao) || (mood = 100 - query_prop(LIVE_FIGHT_MOOD)) < 1)
	/* no parry, i.e. attacker is not current opponent or
	   no OB is used to increase DB */
	return db * (cb_ob_db_mod(0) + mod) / 100;

    if (weap = cb_query_parry_weapon())
    {
	mod += cb_ob_db_mod(1);
	return (query_skill_bonus(weap->query_data()[A_SKILL]) * mood / 100 +
		db) * mod / 100 + weap->query_bonus();
    }
    mod += cb_ob_db_mod(0);
    return (query_skill_bonus(parry_skill) * mood / 100 +
	    db) * mod / 100;
}

/*
 * Function name: cb_query_missile_ob
 * Description:   gives the offensive bonus for missile attacks
 * Arguments:     skill - the skill to query ob for
 *                weap - weapon object, used to query for (magic) bonus
 * Returns:       integer ob
 */
nomask int
cb_query_missile_ob(int skill, object weap)
{
    int mod;

    mod = cb_query_bonus(CB_OB_BONUS);

    if (weap)
    {
	mod += cb_ob_db_mod(1);
	return (weap->query_bonus() + (query_skill_bonus(skill) * mod / 100));
    }
    mod += cb_ob_db_mod(0);
    return (query_skill_bonus(skill) * mod / 100);
}

/*
 * Function name: cb_query_missile_db
 * Description:   gives the defensive bonus against missile attacks
 * Returns:       integer db
 */
nomask int
cb_query_missile_db()
{
    int mod;
    
    mod = cb_query_bonus(CB_DB_BONUS) + cb_ob_db_mod(0);
    return ((query_stat_bonus(STAT_QU) - cb_armor_penalty() +
	     sizeof(shields) * 25) * mod / 100);
}

/*
 * Function name: cb_combat_round
 * Description:   This function handles one combat round
 * Arguments:     attslots - all attack slots
 *                att_count - attack counter
 *                att_speed - speed of single attack
 */
static nomask varargs void
cb_combat_round(int *attslots, int att_count, int att_speed)
{
    int    i, j, sz, f, quick, ao_db, skill, roll, slot;
    object to, weap, *e, *w;
    mixed  attmap, aohloc, wdata;

    to = this_object();
    if (!living(to)) /* this living died */
    {
	ao = 0;
	enemies = ({ });
	delete_state(COMBAT);
	return;
    }
    /* enemy is somehow lost or can't be attacked anymore */
    if (!ao || cb_no_melee_attack(ao))
    {
	ao = 0;
	enemies -= ({ 0 });
	
	/* Switch enemy if we have an alternate */
	e = I(E(this_object()), 2) & enemies;
	for (i=sizeof(e); i--;)
	{
	    if (!cb_no_melee_attack(e[i]))
	    {
		ao = e[i];
		break;
	    }
	}
	if (!ao)
	{
	    /* no more enemies to attack, stop fighting */
	    delete_state(COMBAT);
	    return;
	}
	catch_msg(({ "You turn to attack ", QTNAME(ao), ".\n" }), 0,
		  MSG_SEE);
	ao->catch_msg(({ QCTNAME(to), " turns to attack you.\n" }),
		      to, MSG_SEE);
	tell_watcher(({ QCTNAME(to), " turns to attack ", QTNAME(ao),
			".\n" }));
    }
    quick = cb_query_quickness();
    if (query_prop(LIVE_HASTED) > 0)
    {
	quick /= 2;
	if (att_speed > 3)
	    att_speed /= 2;
	change_int_prop(LIVE_HASTED, -1);
    }
    if (query_prop(LIVE_LOST_ROUNDS) > 0)
    {
	change_int_prop(LIVE_LOST_ROUNDS, -1);
	call_out("cb_combat_round", quick);
	return;
    }
    if ((i = query_state()) & (PARALYSED | UNCONSCIOUS | CONCENTRATE))
    {
	call_out("cb_combat_round", quick);
	return;
    }
    if (i & (SLEEPING | MEDITATING))
    {
	MISC_SOUL->wake("", to, 0);
	if (!query_prop(LIVE_GET_ECHO))
	    catch_tell("You wake up.\n");
	call_out("cb_combat_round", quick);
	return;
    }
    if (i & (ON_GROUND | RESTING))
    {
	command("stand up");
	if (!query_prop(LIVE_GET_ECHO))
	    catch_tell("You stand up.\n");
	call_out("cb_combat_round", quick);
	return;
    }
    if (cb_query_bonus(CB_STUNNED))
    {
	catch_tell("You are stunned, you can only parry or dodge.\n");
	call_out("cb_combat_round", quick);
	return;
    }
    if (!(aohloc = ao->cb_random_hitloc()))
    {
	call_out("cb_combat_round", quick * 2);
	return;
    }    
    if (!att_speed) /* initialize new round */
    {
	if (!(sz = sizeof(attslots = map_indices(attacks))))
	{
	    call_out("cb_combat_round", quick * 2);
	    return;
	}
	for (j=sizeof(w = query_wield(SLT_ALL)); j--;)
	{
	    weap = w[j];
	    for (f=0, i=sz; i--;)
		if (attslots[i] && query_wield(attslots[i]) == weap)
		{
		    /* if twice found it's a more-handed weapon,
		     * remove the attack slot to avoid double usage
		     */
		    if (f) 
			attslots[i] = 0;
		    else
			f = 1;
		}
	    if (f)
		to->skill_check(SK_TWO_WEAPONS, 0, 0);
	}
	attslots -= ({ 0 });

	att_speed = quick / sizeof(attslots);
	if (att_speed < 2)
	    att_speed = 2;
	
	att_count = 0;
    }
    
    if (query_prop(OBJ_INVIS) == 1)
    {
	add_prop(OBJ_INVIS, 0);
	catch_tell("Your invisiblity went off at your attack...\n");
	tell_objects(LISTEN(E(this_object())) - ({ to }),
		     ({ QCTNAME(to),"'s hologram shimmers into view.\n" }),
		     MSG_SEE, to);
    }
	
    slot  = attslots[att_count];
    ao_db = ao->cb_query_melee_db(to);
    
    add_fatigue(-1);
    if (weap = query_wield(slot))
    {
	if (weap->query_prop(IS_SHIELD))
	{
	    if ((++att_count) >= sizeof(attslots))
	    {
		call_out("cb_combat_round", att_speed);
		return;
	    }
	    call_out("cb_combat_round", att_speed, attslots, att_count,
		     att_speed);
	    return;
	}
	wdata = weap->query_data();
	skill = wdata[A_SKILL];
	to->skill_check(skill, 0, 0);
	if ((roll = roll_dice()) <= wdata[A_FRANGE])
	{
	    call_out("cb_combat_round", quick);
	    FUMBLE(skill, weap, this_object(), ao);
	    return;
	}
	cb_message(weap, aohloc, cb_query_melee_ob(skill, weap) +
		   roll - ao_db);
    }
    else if (attmap = attacks[slot])
    {
	skill = attmap[A_SKILL];
	to->skill_check(skill, 0, 0);
	if ((roll = roll_dice()) <= 1)
	{
	    call_out("cb_combat_round", quick);
	    FUMBLE(skill, slot, this_object(), ao);
	    return;
	}
	cb_message(slot, aohloc, cb_query_melee_ob(skill, 0) +
		   roll - ao_db);
    }
    if (!ao || (++att_count) >= sizeof(attslots))
    {
	call_out("cb_combat_round", att_speed);
	return;
    }
    call_out("cb_combat_round", att_speed, attslots, att_count, att_speed);
}

/*
 * Function name: combatstat
 * Description:   info page about combat status
 * Returns:       string description
 */
nomask string
combatstat()
{
    int    i, j, sz, best, num, *ind;
    string str, atype, sep;
    object w, *armor;
    mixed  cur;
    
    str = sprintf("\n%./-35s..%./-40s\n\nQuickness: %-4d" +
		  " |    melee    |   missile   |     Critical threshold  \n"+
		  "Attacks         | OffB | DefB | OffB | DefB | Skin |"+
		  " SLth | RLth | Chain| Plate\n================|======|==="+
		  "===|======|======|======|======|======|======|======\n",
		  capitalize(query_name()), object_name(this_object()),
		  cb_query_quickness());
    ind = map_indices(attacks);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if (w = query_wield(ind[i]))
	{
	    cur = w->query_data();
	    str += sprintf("%_/-15s | %4d | %4d | %4d | %4d | %4d | %4d " +
			   "| %4d | %4d | %4d\n",
			   w->short(),
			   cb_query_melee_ob(cur[A_SKILL], w),
			   cb_query_melee_db(0),
			   cb_query_missile_ob(cur[A_SKILL], w),
			   cb_query_missile_db(),
			   cur[A_SKIN],
			   cur[A_S_LEATHER],
			   cur[A_R_LEATHER],
			   cur[A_CHAIN],
			   cur[A_PLATE]);
	    continue;
	}
	cur = attacks[ind[i]];
	str += sprintf("%_/-15s | %4d | %4d | %4d | %4d | %4d | %4d " +
		       "| %4d | %4d | %4d\n",
		       query_slot_desc(ind[i]),
		       cb_query_melee_ob(cur[A_SKILL], 0),
		       cb_query_melee_db(0),
		       cb_query_missile_ob(cur[A_SKILL], 0),
		       cb_query_missile_db(),
		       cur[A_SKIN],
		       cur[A_S_LEATHER],
		       cur[A_R_LEATHER],
		       cur[A_CHAIN],
		       cur[A_PLATE]);
    }
    str += ("\nHitlocs         |   Best AType \\\\   Protections | Hitchance"+
	    "\n===============================//===============|=="+
	    "=====================\n");
    ind = map_indices(hitlocs);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	cur = hitlocs[ind[i]];
	best = cur[0];
	num = sizeof(armor = query_protection(ind[i]));
	for (j=sizeof(armor); j--;)
	{
	    if (armor[j]->query_at() > best)
		best = armor[j]->query_at();
	}
	switch (best)
	{
	  case A_SKIN:
	      atype = "skin"; break;
	  case A_S_LEATHER:
	      atype = "s. leather"; break;
	  case A_R_LEATHER:
	      atype = "r. leather"; break;
	  case A_CHAIN:
	      atype = "chain"; break;
	  case A_PLATE:
	      atype = "plate"; break;
	  default:
	      atype = "unknown";
	}
	if (sep == "\\\\")
	    sep = "//";
	else
	    sep = "\\\\";
	str += sprintf("%_/-15s | %11s  "+sep+
		       "         %3d   |    %4d\n",
		       query_slot_desc(ind[i]),
		       atype,
		       num, 
		       cur[1]);
    }		       
    return str + "\n";
}
