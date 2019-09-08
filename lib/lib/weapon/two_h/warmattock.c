inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("warmattock");
    add_name("mattock");
    set_short("warmattock");
    set_pshort("warmattocks");
    set_long("A large warmattock with an iron edge and a wooden shaft.\n");
    set_data(SK_ATTACK_WAR_MATTOCK, ATMOD_WAR_MATTOCK, OBMOD_WAR_MATTOCK, 6);
    add_lwvs(0, 2700, 675, 50);
    set_wield_size(2300, 2);
    set_value(1500);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_WAR_MATTOCK : 25]) );
}
