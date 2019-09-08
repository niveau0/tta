inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("handaxe");
    add_name("axe");
    set_short("handaxe");
    set_pshort("handaxes");
    set_long("A small handaxe made of fine steel.\n");
    set_data(SK_ATTACK_HAND_AXE, ATMOD_HAND_AXE, OBMOD_HAND_AXE, 4);
    add_lwvs(0, 2250, 563, 32);
    set_wield_size(1300, 2);
    set_value(500);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_HAND_AXE : 25]) );
}

int
query_throw_skill()
{
    return SK_ATTACK_HAND_AXE_THR;
}
