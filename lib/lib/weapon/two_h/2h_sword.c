inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("sword");
    add_name("blade");
    add_adj("twohand");
    set_short("twohand sword");
    set_pshort("twohand swords");
    set_long("A fine crafted twohand sword of good quality.\n");
    set_data(SK_ATTACK_2_HAND_SWORD, ATMOD_2_HAND_SWORD,
	     OBMOD_2_HAND_SWORD, 5);
    add_lwvs(0, 3850, 963, 60);
    set_wield_size(2700, 2);
    set_value(2000);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_2_HAND_SWORD : 25]) );
}
