inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("falchion");
    add_name("sword");
    add_name("blade");
    set_short("falchion");
    set_pshort("falchions");
    set_long("A fine made falchion of good steel.\n");
    set_data(SK_ATTACK_FALCHION, ATMOD_FALCHION, OBMOD_FALCHION, 5);
    add_lwvs(0, 2000, 500, 30);
    set_wield_size(1600, 2);
    set_value(1500);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_FALCHION : 25]) );
}
