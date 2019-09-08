inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("flail");
    set_short("flail");
    set_pshort("flails");
    set_long("A fine made long flail.\n");
    set_data(SK_ATTACK_FLAIL, ATMOD_FLAIL, OBMOD_FLAIL, 8);
    add_lwvs(0, 2700, 675, 38);
    set_wield_size(2500, 2);
    set_value(1900);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_FLAIL : 25]) );
}
