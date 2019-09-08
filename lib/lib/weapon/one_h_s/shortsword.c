inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("shortsword");
    add_name("sword");
    add_name("blade");
    set_short("shortsword");
    set_pshort("shortswords");
    set_long("A sharp shortsword made of steel.\n");
    set_data(SK_ATTACK_SHORTSWORD, ATMOD_SHORTSWORD, OBMOD_SHORTSWORD, 2);
    add_lwvs(0, 1350, 338, 21);
    set_wield_size(1100, 2);
    set_value(700);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_SHORTSWORD : 25]) );
}
