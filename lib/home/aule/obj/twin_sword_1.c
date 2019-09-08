inherit "/base/weapon";

#include <combat.h>
#include <event.h>
#include <macros.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static object brother;

static void
create_weapon()
{
    set_name("longsword");
    add_name("sword");
    add_name("blade");
    set_name("yak");
    add_adj("black");
    set_short("black longsword");
    set_pshort("black longswords");
    set_long("A black longsword. Some runes are engraved on it and you feel "+
	     "the\n power of ancient magic from it.\n");
    set_data(SK_ATTACK_LONGSWORD, ATMOD_LONGSWORD, OBMOD_LONGSWORD, 3);
    add_lwvs(0, 2000, 500, 33);
    set_wield_size(1600, 2);
    set_value(111800);
    add_prop(OBJ_MATERIAL, M_ENCH_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_LONGSWORD : 1]) );
    add_event(E_WIELD, "object_wielded", TO());
    add_event(E_REMOVE_TOOL, "object_removed", TO());
}

nomask void
object_wielded(object tp, object ob)
{
    object *wielded;
    int i;
    
    if (!tp || ob != TO())
	return;
    wielded = tp->query_wield(SLT_ALL);
    for (i=sizeof(wielded); i--;)
    {
	if (MASTER_OB(wielded[i]) == "/home/aule/obj/twin_sword_2")
	{
	    tp->catch_tell("Your two black swords sing with a humming " +
			   "sound.\n");
	    brother=wielded[i];
	    return;
	}
    }
    call_out("no_brother", 25, tp);	
}

nomask void
object_removed(object tp, object ob)
{
    if (!tp || ob != TO())
	return;

    call_out("call_brother", random(10)+5, tp);
}

void
no_brother(object tp)
{
    object *wielded;
    int i;

    if (!tp || !query_wielder())
	return;
    
    wielded = tp->query_wield(SLT_ALL);
    for (i=sizeof(wielded); i--;)
    {
	if (MASTER_OB(wielded[i]) == "/home/aule/obj/twin_sword_2")
	{
	    brother = wielded[i];
	    return;
	}
    }
    switch(random(3))
    {
      case 1:
	  tp->catch_tell("Your wielded sword cries for something.\n");
	  break;
      case 2:
	  tp->catch_tell("Your wielded sword vibrates in your hand.\n");
	  break;
      default:
	  tp->catch_tell("Your wielded sword mourns something about being "+
		   "alone.\n");
	  break;
    }
    call_out("no_brother", random(100)+25, tp);
}

static void
call_brother(object tp)
{
    if (!brother)
	return;
    
    brother->no_brother(tp);
}
