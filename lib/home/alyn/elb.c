inherit "/base/missile_weapon";

#include <event.h>
#include <combat.h>
#include <material.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>
#include <weapon.h>
#include <macros.h>

static void
create_missile_weapon()
{
    set_name("Elb");
    add_name("elb");
    add_name("bow");
    add_name("composite");
    add_adj("composite");
    add_adj("magical");
    add_adj("alyn's");
    set_short("composite bow");
    set_pshort("bows");
    set_long_bs("A fine crafted bow made of strange woods. "+
                "It has a ruby set into the wood where it is held.\n");
    add_lwvs(0, 950, 329, 53);
    set_wield_size(2700, -2);

    set_data(SK_ATTACK_QUARTERSTAFF, ATMOD_QUARTERSTAFF,OBMOD_QUARTERSTAFF,4);
    set_missile_data(SK_ATTACK_COMPOSITEBOW,
		     ({ 15, 60, 65, 70, 70, 80, ({ PUNCTURE }) }),
		     OBMOD_COMPOSITEBOW, 3);
    
    set_missile_type(MISSILE_ARROW);
    set_weapon_range(5000);
    set_value(5700);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(OBJ_MATERIAL, M_RUBY);
    add_prop(OBJ_NEVER_BREAK, 1);
    add_prop(OBJ_MAGIC, 1);    
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_COMPOSITEBOW : 25]) );
    add_event(E_MOVE,"getting", this_object());
    add_event(E_DID_HIT,"hitting", this_object());
}

static void
getting(object obj, object from, object to)
{
    if (!to || !living(to))
	return;   
    if(to->query_name() == "alyn")
    {
	to->catch_tell("Yes, this is your beloved Elb.\n");
    }
    else
	if (to->query_creator_level() == 0)
	{
	    to->set_hp(0);
	    to->set_fatigue(0);
	    call_out("drop_elb",0,to);
	}
        else
	{
	    to->catch_tell("You know that this is Alyn's bow named Elb. " +
                           "Please give it back to her as soon as possible.\n");
       	}
}

static void
drop_elb(object player)
{
    tell_objects(LISTEN(E(player)) - ({ player }),
		 ({ QCTNAME(player), " tries to get a bow and is struck by " +
			"a red lightning bolt coming from the grip.\n" }),
		 MSG_SEE);
    player->catch_tell("Oh dear, you should have left the bow alone. "+
		   "A red lightning bolt shoots out of the ruby at the grip "+
		   "and hits your chest. All you can feel is pain now.\n",1);    
    player->command("drop alyn's elb");
}

static void
hitting(object arrow, object target, int damage, int slot, object bow)
{
    debug("alyn",({arrow,target,damage,slot,bow}));
}
