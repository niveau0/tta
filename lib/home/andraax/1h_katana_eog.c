inherit "/base/weapon";

#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <material.h>
#include <properties.h>

static void
create_weapon()
{
    set_name("katana");
    add_name("sword");
    add_name("blade");
    set_short("katana");
    set_pshort("katanas");
    set_long("Andraax katana of eog.\n");
    set_data(SK_ATTACK_LONGSWORD, ATMOD_LONGSWORD, OBMOD_LONGSWORD, 1);
    add_lwvs(0, 1800, 480, 30);
    set_wield_size(1500, 2);
    set_value(10000);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_LONGSWORD : 50]) );
}
