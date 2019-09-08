inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("broadsword");
    add_name("sword");
    add_name("blade");
    set_short("broadsword");
    set_pshort("broadswords");
    set_long("An iron broadsword of good quality.\n");
    set_data(SK_ATTACK_BROADSWORD, ATMOD_BROADSWORD, OBMOD_BROADSWORD, 3);
    add_lwvs(0, 1950, 488, 35);
    set_wield_size(1500, 2);
    set_value(1000);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BROADSWORD : 25]) );
}
