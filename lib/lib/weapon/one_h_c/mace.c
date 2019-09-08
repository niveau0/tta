inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("mace");
    add_adj("iron");
    set_short("mace");
    set_pshort("maces");
    set_long("A heavy iron mace of good quality.\n");
    set_data(SK_ATTACK_MACE, ATMOD_MACE, OBMOD_MACE, 2);
    add_lwvs(0, 2600, 650, 30);
    set_wield_size(1700, 2);
    set_value(600);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_MACE : 25]) );
}
