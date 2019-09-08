/* The temperature doesn't really contain a number of degree Celsius or Fahrenheit
 * but a less sharp defined amount of 'thermal energy'. This energy is consumed
 * during the process of damaging the objects structure, but, under certain
 * circumstances, also produced in the process of burning. The damaging-process is mainly
 * calculated upon the weight of the object. Values like volume or size are calculated
 * depending on the weight. 
 * A table containing the material-specific abilities is at /include/burn.h
 */

/* Ideas for extensions:
 * - neighboured object in room also gets hot according to its distance from heatsource
 * - different handling for living object
 * - create an 'ashes'-object and replace the burnt object
 * - temperature should adapt to room's temperature / room-temperatures are a special case
 */

#include <burn.h>
#include <event.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

/* prototyping for /base/object.c */
int query_weight();
int query_volume();
int query_size();
nomask mixed query_prop(mixed prop);
nomask void add_lwvs(int l, int w, int v, int s);
nomask void set_lwvs(int l, int w, int v, int s);
nomask void add_prop(mixed prop, mixed val);

/* prototyping for /system/inventory.c */
nomask varargs object * inventory(int flag);

/* prototyping for burn.c (this code right here) */
int check_fire();
void distribute_heat();
void do_burn_damage();

private int temperature, /* current amount of thermal energy of the object */
    old_temperature, /* previous amount of thermal energy */
    burning, /* contains the id of the callout when being hot enough to get damage */
    /* contains 0 when no callout is running and -1 if last call_out had ended */
    threshold, /* energy threshold beyond which damage is done */
    light_change, /* currently emitted light */
    material,
    burn_initialized,
    org_weight,
    org_volume,
    org_size;

/*
 * Function name: burn_init
 * Description:   stores the original values of the object for later usage
 */
void
burn_init()
{
    org_weight = query_weight();
    org_volume = query_volume();
    org_size = query_size();
    burn_initialized = 1;
}

int
query_temperature()
{
    return temperature;
}

void
set_temperature(int new_temperature)
{
    old_temperature = temperature;
    temperature = new_temperature;
    check_fire();
}

void
add_temperature(int temperature_diff)
{
    old_temperature = temperature;
    temperature += temperature_diff;
    check_fire();
}

/*
 * Function name: query_burning
 * Description:   returns wether object is currently burning or not
 * Returns:       1 if object burns, 0 else 
 */
int
query_burning()
{
    if (burning > 0)
	return 1;
    else
	return 0;
}

/*
 * Function name: check_fire
 * Description:   will test, wether object gets damaged and calculate damage;
 *                it will also create a callout for a continous thermal damage if
 *                necessary
 * Returns:       1 - if object is hot enough to catch fire
 *                0 - object not hot enough to catch fire
 *               -1 - some stupid kind of error occured
 */
int
check_fire()
{
    object env, to;
    
    if (!(material = TO()->query_prop(OBJ_MATERIAL)))
	return -1;
    if (!(threshold = BURN_TABLE[material][BURN_THRESHOLD]))
	return -1;

    if (!burn_initialized)
	burn_init();
    to = this_object();
    
    if (living(to))
    {
	/* Maybe the threshold has to be modified due to livings abilities? */
	/*threshold += FIRE-RESISTANCE?;*/
    }
    
    if( temperature < threshold)
    {
	/* object not hot enough to get damaged by heat */
	if(living(to))
	{
	    to->catch_tell("You feel yourself "+
			   ((old_temperature<temperature)?"heating up":"cooling down")+
			   ".\n");
	}
	if (burning)
	{
	    /* previously burning object is now just hot */
	    env = E(to);
	    
	    if(living(env))
	    {
		env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "extinguishes.\n");
	    }
	    else
	    {
		tell_objects(LISTEN(env) - ({ to }),
			 ({ QCTNAME(to), " extinguishes.\n" }), MSG_SEE, to);
	    }
	    add_lwvs(-light_change, 0, 0, 0);
	}
	remove_call_out(burning);
	burning = 0;
	/* distribute energy within own inventory */
	distribute_heat();
	return 0;
    }
    else
    {
	/* object IS hot enough to get damaged */
	do_burn_damage();
	
	if(burning <= 0)
	{
	    /* object wasn't burning until now but continues burning */
	    burning = call_out("continue_fire", BURN_TABLE[material][BURN_TIME]);
	}
	return 1;
    }
}

/*
 * Function name: continue_fire
 * Description:   starting point for a burning-call_out
 *                shows via 'burning' that currently no call_out is running
 */
void
continue_fire()
{
    burning = -1;
    distribute_heat();
    check_fire();
}

/*
 * Function name: do_burn_damage
 * Description:   the function will handle all effects the current temperature
 *                might cause at the object
 */
void
do_burn_damage()
{
    int grams_to_burn, /* material that will burn due to current heat */
	energy_emission, /* while burning process lost energy (source of light) */
	old_light_change; /* previously emitted light */
    float damage_ratio; /* ratio between actual and original weight/volume/size */
    object to, env;
    
    to = this_object();
    env = E(to);
    
    grams_to_burn = (int)temperature/threshold;
    
    if (grams_to_burn >= query_weight())
    {
	/* object is fully destroyed */
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+
			    " in your hands turns into ashes.\n");
	}
	else
	{

	    tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " turns into ashes.\n" }), MSG_SEE, to);
	}
	if (living(to))
	{
	    to->catch_tell("You turned into a cloud of smoke!\n");
	    /* death has to be caused somehow? */
	    return;
	}
        this_object()->remove_object();
    }

    old_temperature = temperature;
    /* energy used up in damaging object */
    temperature -= grams_to_burn * BURN_TABLE[material][BURN_THRESHOLD];
    /* produced energy due to burning */
    temperature += grams_to_burn *  BURN_TABLE[material][BURN_ENERGY];
    /* the two former formulas might be combined -> faster computing */
    
    /* energy-loss due to light(all frequencies)-emission */
    energy_emission = temperature*BURN_TABLE[material][ENERGY_LOSS]/100;
    temperature -= energy_emission;
    old_light_change = light_change;
    light_change = (int)((float)energy_emission*ENERGY_TO_LIGHT);

    if (light_change > 0)
    {
	/* Object emits light, therefore it has to be burning. */
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+" in your hands "+
			    ((burning!=0)?"burns on":"catches fire")+".\n");
	}
	else
	{
	    tell_objects(LISTEN(env) - ({ to }),
			 ({ QCTNAME(to), ((burning!=0)?" burns on":" catches fire")+".\n" }),
			 MSG_SEE, to);
	}
    }
    else
    {
	/* Though object is damaged, it does't emit light. It's not burning. */
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+" in your hands "+
			    "is damaged by its heat.\n");
	}
	else
	{
	    tell_objects(LISTEN(env) - ({ to }),
			 ({ QCTNAME(to), " is damaged by its heat.\n" }), MSG_SEE, to);
	}
    }
    
    if (living(to))
    {
	/* damaged object is alive and feels the pain */
	to->catch_tell("The intense heat hurts you!\n");
	/* inflict damage to living here... */
	return;
    }

    /* lost volume and size due to damaging object */
    /* this is handled differently in case of living object */
    damage_ratio = ((float)(query_weight()-grams_to_burn)/(float)org_weight);
    add_lwvs(light_change - old_light_change,
	     -grams_to_burn,
	     ( (int)((float)org_volume*damage_ratio)-query_volume() ),
	     ( (int)((float)org_size*damage_ratio)-query_size() ) );
    /* setting the damage-property */
    add_prop(OBJ_DAMAGED, 100-(int)(damage_ratio*100.0));
}

/*
 * Function name: distribute_heat
 * Description:   will distribute energy among close (same inventory)
 *                objects evenly
 */
void
distribute_heat()
{
    int	heat_drain, drain_objects, i;
    object to, env, *neighb_objects;

    to = this_object();
    env = E(to);
    
    /* energy gets transferred to/from neighboured objects */
    neighb_objects = to->inventory(0);
    if (!query_prop(IS_ROOM))
    {
	/* distribute among objects in own inventory and itself */
        heat_drain = temperature;
	drain_objects = sizeof(neighb_objects)-1;
	/* compute the maximum energy pool */
        for (i = drain_objects; i >= 0; i--)
        {
	    heat_drain += neighb_objects[i]->query_temperature();
        }
	
	if (drain_objects >= 0)
	    heat_drain = heat_drain/(drain_objects+1); /* +1 for itself */
	/* set the energy for neighbours/environment/object */
        for (i = drain_objects; i >= 0; i--)
        {
	    neighb_objects[i]->set_temperature(heat_drain);
        }
	temperature = heat_drain;
	env->check_fire();
    }
}
