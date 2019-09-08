inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("cola");
    add_adj("cola flasche");
    set_short("cola");
    set_pshort("cola flaschen");
    set_long("Eine Cola Flasche mit nem erfrischungs Fenster.\n");
    set_data(SK_ATTACK_BATTLE_AXE, ATMOD_BATTLE_AXE, OBMOD_BATTLE_AXE, 5);
    add_lwvs(0, 3150, 788, 38);
    set_wield_size(2700, 1);
    set_value(1300);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BATTLE_AXE : 25]) );
}
