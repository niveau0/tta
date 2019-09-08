#include <event.h>
#include <macros.h>

int try_to_wield_weapon(object wielder, object weapon);

nomask void
react_on_attack(object target, object attacker)
{
    int *att_co, *tar_co, step;
    object melee, range;

    if (!target || !attacker || !living(attacker) || !living(target))
	return;

    // wenn der attacker invisible ist (oder hidden) -> flee ?
    if (!VISIBLE(target, attacker))
    {
	target->command("shout Where are you wimpy attacker?");
	debug("aule", "can't see attacker");
	return;
    }
    
    remove_event(E_ATTACKED_BY, "react_on_attack", target);
    
    att_co = attacker->query_coordinates();
    tar_co = target->query_coordinates();
    step = target->query_step();

    if (!distance(tar_co, att_co, 10*step))
    {
	// enternung korrekt ?? auf fernkampfwaffe abstimmen ?
	range = target->query_pref_range_weapon();
	debug ("aule", "Fernkampf:");
	if (!range)
	{
	    debug("aule", "keine range waffe");
	    target->command("say I will kill you for that!");
	    melee = target->query_pref_melee_weapon();
	    try_to_wield_weapon(target, melee);
	    target->command( "run "+QTNAME(attacker)[1]);
	    /* nu wenn moeglich Magie, dann Nahkampfwaffe ziehen
	       und auf Feind zurennen - oder fluechten/verstecken */
	}
	else
	{
	    debug("aule", "range weapon wielden");
	    if (!try_to_wield_weapon(target, range))
	    {
		//wielden ok
		//shoot back (range fight - liste wie bei melee ?)
	    }
	}
    }
    else
    {
	if (!distance(tar_co, att_co, 2*step))
	{
	    // halbdistanz, (entfernung abstimmen ?)
	    // potions trinken, magie, waffe wechseln etc
	    target->command( ({"run ",QTNAME(attacker)[1]}));
	}
	melee = target->query_pref_melee_weapon();
	debug ("aule", "Nahkampf");
	/* waehrend des kampfes verschiedene Strategien wie:
	   entwaffnen, Magie(?), kicks, usw (ne liste erstellen
	   schon beim create_object?) */
	if (!melee)
	{
	    debug("aule", "keine melee waffe");
	    target->command("whine");
	}
	else
	{
	    debug("aule", "melee weapon wielden");
	    target->command("say I will kill you for that!");
	    try_to_wield_weapon(target, melee);
	}
    }
}

int
try_to_wield_weapon(object wielder, object weapon)
{
    mixed res;
    
    debug("aule","try_to_wield");
    res = wielder->wield_tool(weapon);

    switch (res)
    {
      case -1:
	  wielder->command("say I can't use the weapon");
	  break;
      case -2:
	  wielder->command("say The weapon is too big");
	  break;
      case -3:
	  wielder->command("say Already wielded the weapon.");
	  return 0;
      case -4:
	  wielder->command("say Can't wield the weapon because of "+
			   "unknow reason.");
	  break;
      default:
	  wielder->command("say OK, weapon wielded.");
	  return 0;
    }
    return -1;
}
