inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("maingauche");
    add_name("gauche");
    set_short("maingauche");
    set_pshort("maingauches");
    set_long("A fine made maingauche of high quality.\n");
    set_data(SK_ATTACK_MAIN_GAUCHE, ATMOD_MAIN_GAUCHE, OBMOD_MAIN_GAUCHE, 2);
    add_lwvs(0, 675, 169, 16);
    set_wield_size(700, 2);
    set_value(1200);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_MAIN_GAUCHE : 25]) );
}
