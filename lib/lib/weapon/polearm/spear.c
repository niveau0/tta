inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("spear");
    set_short("spear");
    set_pshort("spears");
    set_long("A long, pointed spear.\n");
    set_data(SK_ATTACK_SPEAR, ATMOD_SPEAR, OBMOD_SPEAR, 5);
    add_lwvs(0, 2500, 714, 89);
    set_wield_size(3000, 2);
    set_value(230);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_SPEAR : 25]) );
}

int
query_throw_skill()
{
    return SK_ATTACK_SPEAR_THR;
}
