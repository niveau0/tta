inherit "/base/shield";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_shield()
{
    set_name("shield");
    add_adj("metal");
    set_short("metal shield");
    set_pshort("metal shields");
    set_long("A fine made shield of good steel.\n");
    add_lwvs(0, 8000, 5000, 44);
    set_data(SK_ATTACK_BASH, ATMOD_BASH, OBMOD_BASH, 6);
    set_wield_size(1900, 2);
    set_value(2000);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BASH : 5]) );
}
