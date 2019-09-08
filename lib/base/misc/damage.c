/*
 * this file holds general damage functions.
 * for combat macros see <combat.h>
 */

#include <combat.h>
#include <macros.h>
#include <properties.h>

/* used to tune global damage */
#define CONCUSSION_MOD 10

nomask void
create()
{
    if (IS_CLONE)
	destruct_object();
}

/*
 * Function name: damage_living
 * Description:   compute and return damage against a living,
 *                also call critical
 * Arguments:     att - attacking object
 *                ao - target object
 *                diff - difference ob - db
 *                at_data - attack data mapping
 *                hloc - the hitlocation
 *                       ({ slot, description, armortype, hit chance })
 * Returns:       integer (damage)
 */
nomask int
damage_living(object att, object ao, int diff, mapping at_data, mixed hloc)
{
    int    i, damage, atype, threshold, damage_mod, tmp, *c;
    object *armor;
    
    if (diff < 0)
	return 0;
    
    damage_mod = at_data[A_DMOD];  /* damage modifier */
    armor = ao->query_protection(hloc[HITLOC_SLOT]);
    threshold = at_data[hloc[HITLOC_AT]];

    /* get effective threshold and total damage & ob modifier */
    for (i=sizeof(armor); i--;)
    {
	if (!(atype = armor[i]->query_at()))
	    continue; /* is equipment, not armor */
	switch (atype)
	{
	  case A_SKIN:      damage_mod += DMOD_SKIN; break;
	  case A_S_LEATHER: damage_mod += DMOD_S_LEATHER; break;
	  case A_R_LEATHER: damage_mod += DMOD_R_LEATHER; break;
	  case A_CHAIN:     damage_mod += DMOD_CHAIN; break;
	  case A_PLATE:     damage_mod += DMOD_PLATE; break;
	}
	if (threshold < at_data[atype])
	    /* find threshold of best armor */
	    threshold = at_data[atype];
	else
	    /* increase	threshold for the extra protection */
 	    tmp += 5;
    }
    threshold += tmp;
    if (damage_mod < 1) /* should never happen, but who knows */
	damage_mod = 1;
    damage = diff * CONCUSSION_MOD / damage_mod;
    /*    debug("manwe", "Thres:" + threshold + " Diff:" + diff +
    	  " Dam:" + damage + " DamMod:" + damage_mod);*/
    debug("orome", "Thres:" + threshold + " Diff:" + diff +
    	  " Dam:" + damage + " DamMod:" + damage_mod);
    
    if (diff > threshold && !att->query_prop(LIVE_NO_CRITICAL) &&
	sizeof(c = at_data[A_CRIT]))
	damage += CRITICAL(c[random(sizeof(c))], hloc, ao, att,
			   diff, threshold);
    
    return damage;
}
