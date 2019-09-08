inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("longsword");
    add_name("sword");
    add_name("blade");
    set_short("longsword");
    set_pshort("longswords");
    set_long("A sharp longsword made of steel.\n");
    set_data(SK_ATTACK_LONGSWORD, ATMOD_LONGSWORD, OBMOD_LONGSWORD, 3);
    add_lwvs(0, 2000, 500, 33);
    set_wield_size(1600, 2);
    set_value(1800);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_LONGSWORD : 25]) );
}
