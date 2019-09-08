inherit "/base/weapon";

#include <combat.h>
#include <event.h>
#include <material.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>
#include <weapon.h>

static void drop_hammer(object dest);

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
    set_data(SK_ATTACK_WAR_HAMMER, ATMOD_WAR_HAMMER, OBMOD_WAR_HAMMER, 2);
    add_lwvs(0, 500, 125, 33);
    set_wield_size(1, 2);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_WAR_HAMMER : 500]) );
    add_event(E_MOVE, "moving", this_object());
}

static void
drop_hammer(object dest)
{
    dest->set_hp(1);
    tell_objects(LISTEN(E(dest)) - ({ dest }),
		 ({ QCTNAME(dest), " takes the fine warhammer, "+
			"but is suddenly engulfed by electricity and "+
			"instantly drops it again.\n"}),
		 MSG_SEE);
    dest->catch_tell("You are sure you made a mistake. This weapon is "+
		     "not made for your hands.\nA lightning bolt runs "+
		     "through your body, and you instantly drop the "+
		     "hammer.\n");
}
