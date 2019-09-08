inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <slots.h>
#include <skills.h>
#include <properties.h>

static void
create_weapon()
{
    set_name("nodachi");
    add_name("katana");
    add_adj("twohand");
    set_short("twohand katana");
    set_pshort("twohand katanas");
    set_long("Andraax nodachi of Eog.\n");
    set_data(SK_ATTACK_2_HAND_SWORD, ATMOD_2_HAND_SWORD, OBMOD_2_HAND_SWORD,2);
    add_lwvs(0, 3350, 780, 54);
    set_wield_size(2100, 2);
    set_value(20000);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_2_HAND_SWORD : 200]) );
}
