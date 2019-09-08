inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("warhammer");
    add_name("hammer");
    set_short("warhammer");
    set_pshort("warhammers");
    set_long ("A heavy iron warhammer.\n");
    set_data(SK_ATTACK_WAR_HAMMER, ATMOD_WAR_HAMMER, OBMOD_WAR_HAMMER, 4);
    add_lwvs(0, 2500, 625, 33);
    set_wield_size(1800, 2);
    set_value(1500);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_WAR_HAMMER : 25]) );
}
