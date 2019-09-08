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
    add_adj("hide");
    set_short("leather shield");
    set_pshort("leather shields");
    set_long("A shield with a piece of blackened hide.\n");
    add_lwvs(0, 1000, 5000, 44);
    set_data(SK_ATTACK_BASH, ATMOD_BASH, OBMOD_BASH, 6);
    set_wield_size(1900, 2);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BASH : 5]) );
}
