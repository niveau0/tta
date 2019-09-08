inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("glamdring");
    set_short("Glamdring");
    set_long("Glamdring - the Foe-Hammer.\n");
    add_lwvs(1,100,100,10);
    set_wield_size(100, 2);
    set_value(10000);
    add_prop(OBJ_MATERIAL, M_STEEL);
}

int
query_throw_skill()
{
    return SK_ATTACK_SPEAR_THR;
}
