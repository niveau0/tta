//inherit "/base/object";

#include <burn.h>
#include <event.h>
#include <macros.h>
#include <material.h>
#include <properties.h>


int query_weight();
nomask void add_lwvs(int l, int w, int v, int s);

private int threshold, energy, emission_ratio, material, burn_time,
    temperature, light_change, is_burning;

int
start_fire(int start_temp)
{
    object env, to;
    
    if (!(material = TO()->query_prop(OBJ_MATERIAL)))
	return -1;
	
    to = this_object();
    env = E(to);

    threshold = BURN_TEMP[material][BURN_THRESHOLD];
    energy = BURN_TEMP[material][BURN_ENERGY];
    burn_time = BURN_TEMP[material][BURN_TIME];
    emission_ratio =  BURN_TEMP[material][ENERGY_LOSS];
    
    if( (start_temp + temperature) < threshold)
    {
	// object not hot enough to start burning
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "heats up.\n");
	}
	else
	{
	    tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " heats up.\n" }),
		     MSG_SEE, this_object());
	}
	temperature += start_temp;
    }
    else
    {
	// object is hot enough to catch fire ...
	if(is_burning)
	{
	    //object is already burning
	    if(living(env))
	    {
		env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "burns brighter!\n");
	    }
	    else
	    {
		tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " burns even stronger!\n" }),
		     MSG_SEE, to);
	    }
	    temperature += start_temp;
	}
	else
	{
	    if(living(env))
	    {
		env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "starts to burn!\n");
	    }
	    else
	    {
		tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " starts to burn!\n" }),
		     MSG_SEE, to);
	    }
	    is_burning = 1;
	    return call_out("compute_dam",
			    burn_time, start_temp + temperature);
	}
    }
    return 1;
}

int
compute_dam(int old_temp)
{
    int grams_to_burn, energy_emission;
    object env, to;

    to = this_object();
    env = E(to);
    grams_to_burn = (int)old_temp/threshold;
    
    if (grams_to_burn >= query_weight())
    {
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "burns to ashes.\n");
	}
	else
	{

	    tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " burns to ashes.\n" }),
		     MSG_SEE, to);
	}
        this_object()->remove_object();
	return 1;
    }
    

    if(living(env))
    {
	env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "burns on.\n");
    }
    else
    {
	tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " burns.\n" }),
		     MSG_SEE, to);
    }

    //energy changes due to burning
    temperature = (old_temp - grams_to_burn * (threshold - energy));

    //energy-loss and light-emission
    energy_emission = temperature*emission_ratio/100;
    temperature -= energy_emission;
    light_change = (int)energy_emission*emission_ratio - light_change;
    
    add_lwvs(light_change, -grams_to_burn, 0, 0);
    if( temperature >= threshold )
	return call_out("compute_dam", burn_time, temperature);
    else
    {
	if(living(env))
	{
	    env->catch_tell("The "+to->query_name()+" in your inventory "+
			    "extinguishes.\n");
	}
	else
	{
	    tell_objects(LISTEN(env) - ({ to }),
		     ({ QCTNAME(to), " extinguishes.\n" }),
		     MSG_SEE, to);
	}
	is_burning = 0;
	return -1;
    }
}

int
get_temperature()
{
    return temperature;
}

void
set_temperature(int new_temperature)
{
    temperature = new_temperature;
}
