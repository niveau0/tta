inherit "../spell_base";

#include <composite.h>
#include <macros.h>
#include <magic.h>
#include <material.h>
#include <rr_types.h>
#include <properties.h>
#include <slots.h>

/*
 * Function name: test_success
 * Description:   check spell success, handles fumbles and mana cost
 * Arguments:     tp - casting mage
 *                clev - casting level
 *                knownlev - learned listlevel
 *                type - spell type
 *                prep - how long spell has been prepared
 * Returns:       1 if successful
 */
int
test_success(object tp, int clev, int knownlev, int type, int prep)
{
    int    i, sz, fail, roll;
    object *obj;

    if (tp->query_mana() < clev)
    {
	tp->catch_tell("You are mentally too exhausted.\n");
	return 0;
    }
	
    if (clev > knownlev)
    {
	/* above class III spell, needs 2 rounds prep. Additionally
	 * (15% + 5% * each overcast level) failure
	 */
	if (prep < 2)
	    fail += 25;
	else if (prep < 1)
	    fail += 25;
	fail += 15 + (clev - knownlev) * 5;
    }
    else if (clev > knownlev - 3)
    {
	/* class III spell, needs 2 rounds prep. */
	if (prep < 2)
	    fail += 25;
	else if (prep < 1)
	    fail += 25;
    }
    else if (clev > knownlev - 6)
    {
	/* class II spell, needs 1 rounds prep. */
	if (prep < 1)
	    fail += 25;
    }
    /* class I spell, needs no round prep. */

    /* cha-realm casters get penalty for metal protections */
    if (sz = sizeof(obj = tp->query_worn(SLT_ALL)))
    {
	int mat, metalweight;
	
	for (i=0; i<sz; i++)
	{
	    if ((mat = obj[i]->query_prop(OBJ_MATERIAL)) >= M_METAL)
		metalweight += obj[i]->query_weight();
	}
	fail += metalweight / 1000;
    }
    if (roll_dice(-1) < fail)
    {
	if (type & (SP_FORCE | SP_MENTALATTACK))
	    i = SPELL_FUMBLES->fumble_attack(tp, roll_dice() + clev);
	else
	    i = SPELL_FUMBLES->fumble_non_attack(tp, roll_dice() + clev);
	if (!tp)
	    return 0;
	if (i != -1)
	    tp->add_mana(-clev);
	if (i > 0)
	    tp->check_death(i, this_object());
	return 0;
    }
    tp->add_mana(-clev);
    return 1;
}

/*
 * Function name: resist
 * Description:   check the spell resistance against base attack spells
 * Arguments:     tp - casting person
 *                targets - all spell targets
 *                level - attack level
 *                type - spell type
 * Returns:       ({ nonresisting targets })
 */
object *
resist(object tp, object *targets, int level, int type)
{
    int    i, sz, roll, rr_mod;
    object *no_resist;
    
    roll      = random(100) + 1;
    rr_mod    = query_rr_mod();
    no_resist = allocate(sz = sizeof(targets));
    
    if (type & (SP_HEALING | SP_DEFENSIVE | SP_PASSIVE))
	rr_mod -= 50;
    else if (type & (SP_FORCE | SP_MENTALATTACK))
    {
	for (i=0; i<sz; i++)
	    if (VISIBLE(targets[i], tp))
		targets[i]->cb_attack_object(tp);
    }
    for (i=0; i<sz; i++)
    {
	if (targets[i] == tp ||
	    targets[i]->spell_resistance(RR_CHA, level, roll, rr_mod) < 1)
	    no_resist[i] = targets[i];
    }
    no_resist -= ({ 0 });
    targets -= no_resist;
    
    if ((sz = sizeof(targets)) == 1)
	tp->catch_msg(({ QCTNAME(targets[0]), " seems to resist.\n" }),
		      0, MSG_SEE);
    else if (sz > 1)
	tp->catch_msg(COMPOSITE_ARRAY(targets) +
		      ({ " seem to resist.\n" }), 0, 0);
    
    return no_resist;
}
