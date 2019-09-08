inherit "/base/missile";

#include "../../include/moria.h"
#include <macros.h>
#include <material.h>
#include <properties.h>
#include <weapon.h>
#include <effects.h>
#include <skills.h>

private string used_poison;

void
set_poison(string poison)
{
    used_poison = poison;

    if(poison != "" && IS_CLONE)
    {
	add_item(({"head", "paste" }),
		 ({ "heads", "paste" }),
		 "@@poison_desc:"+MASTER+"#"+OB_NUM(this_object()), 1);
	return;
    }
    remove_item(1);
}

string
query_poison()
{
    return used_poison;
}


string
poison_desc()	
{
    string desc;
    object tp, ob;
    int skill;
    
    
    tp = this_player();
    ob = this_object();
    
    skill = tp->query_skill_bonus(SK_POISON_LORE);
    desc = "You identifiy the paste as ";
    if ((roll_dice(-1)+skill) > 80)
	desc = desc + (POISON_DIR+used_poison)->query_desc()+
	       ".\n";
    else
	desc = "You cannot identify that.\n";
    return desc;
}

static void
create_missile()
{
    set_name("bolt");
    add_adj("wooden");
    set_short("wooden bolt");
    set_pshort("wooden bolts");
    set_long("A light wooden bolt with a metal point.\n");
    set_plong("Some light wooden bolts with metal points.\n");
    set_missile_type(MISSILE_BOLT);
    set_heap_size(1);
    set_heap_id(MASTER);
    set_value(1);
    add_lwvs(0, 50, 20, 23);
    add_prop(OBJ_MATERIAL, M_OAK);
    if (IS_CLONE)
	set_poison("acaana");
}

string
long()
{
    string descr;
    
    if(!(query_poison() == "" || (query_poison())))
    {
	if (query_heap_size() == 1)
	    descr = "It's head seems to be tipped into some paste.\n";
	else
	   descr = "The heads seem to be tipped into some paste.\n"; 
    }
    descr = ::long() + ( descr ? descr : "");
    return descr;
}

void
E_did_melee_hit(object who, int damage)
{
    debug("yavanna",damage);
    
    if (2*(random(80))<damage)
    {
	if((query_poison()) || !(query_poison() == ""))
	    (POISON_DIR+query_poison())->poison_player(who,0);
	if (random(100) > 3)
	    set_poison("");
    }
}






