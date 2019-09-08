inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("polearm");
    add_adj("wooden");
    set_short("polearm");
    set_pshort("polearms");
    set_long("A long strong wooden polearm.\n");
    set_data(SK_ATTACK_POLE_ARM, ATMOD_POLE_ARM, OBMOD_POLE_ARM, 7);
    add_lwvs(0, 3850, 963, 124);
    set_wield_size(3000, 2);
    set_value(1400);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_POLE_ARM : 25]) );
}
