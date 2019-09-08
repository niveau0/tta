inherit "/base/weapon";

#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <material.h>
#include <properties.h>

static void
create_weapon()
{
    set_name("wakizashi");
    add_name("sword");
    add_name("katana");
    set_short("wakizashi");
    set_pshort("wakizashis");
    set_long("Andraax wakizashi made of eog.\n");
    set_data(SK_ATTACK_SHORTSWORD, ATMOD_SHORTSWORD, OBMOD_SHORTSWORD, 1);
    add_lwvs(0, 1250, 300, 20);
    set_wield_size(1000, 2);
    set_value(5000);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_SHORTSWORD : 50]) );
}
