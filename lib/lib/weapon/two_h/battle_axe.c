inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("axe");
    add_adj("battle axe");
    set_short("battle axe");
    set_pshort("battle axes");
    set_long("A large battle axe with a sharp blade.\n");
    set_data(SK_ATTACK_BATTLE_AXE, ATMOD_BATTLE_AXE, OBMOD_BATTLE_AXE, 5);
    add_lwvs(0, 3150, 788, 38);
    set_wield_size(2700, 2);
    set_value(1300);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BATTLE_AXE : 25]) );
}
