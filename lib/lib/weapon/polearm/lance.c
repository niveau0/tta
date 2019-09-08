inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("lance");
    add_name("polearm");
    set_short("lance");
    set_pshort("lances");
    set_long("A long, pointed lance of medium quality.\n");
    set_data(SK_ATTACK_LANCE, ATMOD_LANCE, OBMOD_LANCE, 7);
    add_lwvs(0, 5200, 1300, 142);
    set_wield_size(4000, 2);
    set_value(500);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_LANCE : 25]) );
}
