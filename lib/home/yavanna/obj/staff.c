inherit "/base/weapon";

#include <slots.h>
#include <skills.h>
#include <weapon.h>
#include <combat.h>
#include <material.h>
#include <properties.h>
#include <macros.h>
#include "/kernel/include/event.h"

void
create_weapon()
{
    set_name ("staff");
    add_name ("polearm");
    set_pname ("staffs");
	      
    set_short ("wooden staff");
    set_pshort ("wooden staffs");

    set_long (
	"A small but very strong staff that seemed to be naturaly grown.\n"+
	"It belongs to the valie Yavanna.\n"
	);
    
/*    set_weapon_class (W_POLEARM); */
    set_data(SK_ATTACK_POLE_ARM,
	     ({10, 18, 25, 25, 33, 52, ({ IMPACT }) }),
	      36,
	      1);
    add_lwvs (0, 1450, 963, 124);
    set_wield_size(5780, 2);
    set_value (5000);
    add_prop (OBJ_MATERIAL, M_TREANT);
    add_prop (SKILL_SUPPORT, ([SK_ATTACK_POLE_ARM : 45]) );
    add_prop (SKILL_SUPPORT, ([SK_FLORA_LORE : 50]));
    add_prop (OBJ_MAGIC, 10);
    
}

void
set_wielder(object w)
{
    ::set_wielder(w);
    if (w) 
    {
	w->catch_tell("The staff glows for a moment in a bright light and begins "+
		      "to vibrate\n"+
		      "in your hands. It seems to live itself.\n" +
		      "You feel the power of life flowing through your body.\n");
	tell_objects(LISTEN(E(w)) - ({ w }),
		     ({ QCTNAME(w),
			    "'s staff glows for a moment in a bright light and "+
			    "seems to vibrate as if it became alive.\n" }),
		     MSG_SEE, w);
//	w->add_bloodloss(-(w->query_bloodloss()));
//	w->add_prop(LIVE_ONLY_STUN, 1);
	w->add_prop(LIVE_NEVER_ILL, 1);
	w->add_prop(LIVE_NEVER_BLEED, 1);
//	w->add_prop(LIVE_MANA_MULTIPLY, 500);
//	w->add_prop(GOD_NO_CONSUME, 1);
    } 
    if (!w) 
    {
	E(TO())->catch_tell("You feel the Power of life flowing back "+
			    "into the staff.\n");
//	E(TO())->add_prop(LIVE_ONLY_STUN, 0);
	E(TO())->add_prop(LIVE_NEVER_ILL, 0);
	E(TO())->add_prop(LIVE_NEVER_BLEED, 0);
//	E(TO())->add_prop(LIVE_MANA_MULTIPLY, 0);
//	E(TO())->add_prop(GOD_NO_CONSUME, 0);
    }
}

void
E_did_melee_hit(object who, int damage)
{
    CRITICAL(IMPACT, who->cb_random_hitloc(), who, query_wielder(), damage, 19);
}

