inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("rapier");
    set_short("rapier");
    set_pshort("rapiers");
    set_long("A fine made rapier of good steel.\n");
    set_data(SK_ATTACK_RAPIER, ATMOD_RAPIER, OBMOD_RAPIER, 4);
    add_lwvs(0, 1040, 260, 47);
    set_wield_size(1000, 2);
    set_value(2200);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_RAPIER : 25]) );
}
