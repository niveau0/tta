inherit "/base/weapon";

#include <slots.h>
#include <skills.h>
#include <weapon.h>
#include <combat.h>
#include <material.h>
#include <macros.h>
#include <stdproperties.h>

static void
create_weapon()
{
    set_name("warhammer");
    add_adj("fine");
    add_name("hammer"); 
    add_name("eceru");
    set_short("fine warhammer");
    set_pshort("fine warhammers");
    set_long ("Whow. You are very impressed by this weapon. You never saw "+
	      "such a fine\nweapon ever before. This mighty hammer of a "+
	      "Valar is made of gold\ninlaid black eog and its enchanted "+
	      "head is harder than any substance.\nYou are sure. you "+
	      "shouldn't touch this weapon, if you are not Aule.");
    set_weapon_class(W_ONE_HAND_CRUSH);
    set_weapon_skill(SK_ATTACK_WAR_HAMMER);
    add_lwvs(0, 500, 125, 33);
    set_wield_size(1, 2);
    set_at_modifier(32, 47, 42, 50, 77, ({ CRUSH, PUNCTURE }) );
    set_ob_modifier(-5, 0, 0, 10, 10);
    set_fumble_range(1);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_WAR_HAMMER : 500]) );
    add_event(E_MOVE, this_object());
}

static void
E_move(object to, object env, object dest)
{
    if (!dest || !living(dest))
	return ;
    if(dest->query_name() == "aule")
    {
	dest->catch_tell("This weapon feels very good in your hand.\n");
    }
    else if (dest->query_creator_level() < 5)
    {
	dest->set_hp(1);
	call_out("drop_hammer",0, dest);
    }
    else
    {
	dest->catch_tell("You found Aule's hammer named Eceru. Don't give "+
			 "it away, except to Aule.\n");
    }
}
    
static void
drop_hammer(object dest)
{
    if (!dest || E(this_object()) != dest)
	return;
    
    tell_objects(E(dest)->query_listen() - ({ dest }),
		 ({ QCTNAME(dest), " takes the fine warhammer, "+
			"but is suddenly engulfed by electricity and "+
			"instantly drops it again.\n"}),
		 MSG_SEE);
    dest->catch_tell("You are sure you made a mistake. This weapon is "+
		     "not made for your hands.\nA lightning bolt runs "+
		     "through your body, and you instantly drop the "+
		     "hammer.\n");
    set_coordinates(dest->query_coordinates());
    move(E(dest),0);
}
