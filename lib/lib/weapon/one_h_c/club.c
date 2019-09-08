inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("club");
    add_adj("wooden");
    set_short("club");
    set_pshort("clubs");
    set_long ("A wooden club of average size.\n");
    set_data(SK_ATTACK_CLUB, ATMOD_CLUB, OBMOD_CLUB, 4);
    add_lwvs(0, 2300, 657, 47);
    set_wield_size(1600, 2);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_CLUB : 25]) );
}
