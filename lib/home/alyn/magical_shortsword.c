inherit "/base/weapon";

#include <slots.h>
#include <skills.h>
#include <weapon.h>
#include <combat.h>
#include <material.h>
#include <properties.h>

static void
create_weapon()
{
    mixed atmod;
    
    set_name("magical shortsword");
    add_name("shortsword");
    add_name("sword");
    add_adj("magical");
    add_adj("sharp");
    set_short("shortsword");
    set_pshort("shortswords");
    set_long_bs("This is a sharp shortsword made of steel. " +
		"Its nice pointed tip makes it suitable for thrusting. " +
	        "As you examine it closer you see some strange symbols " +
	        "on its blade which could be runes.\n");
    atmod = ATMOD_SHORTSWORD;
    atmod[0] -= 10; //damage mod
    atmod[1] -= 5;  //skin
    atmod[2] -= 5;  //s_leather
    atmod[3] -= 5;  //r_leather
    atmod[4] -= 5;  //chain
    atmod[5] -= 5;  //plate
    set_data(SK_ATTACK_SHORTSWORD, atmod, OBMOD_SHORTSWORD, 2);
    add_lwvs(0, 1150, 338, 21);
    set_wield_size(1000, 2);
    set_value(5000);
    add_prop(OBJ_NEVER_BREAK,1);
    add_prop(OBJ_MAGIC,1);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_SHORTSWORD : 25]) );
}
