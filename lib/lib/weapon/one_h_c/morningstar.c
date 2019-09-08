inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("morningstar");
    set_short("morningstar");
    set_pshort("morningstars");
    set_long("A heavy morningstar with a spiked, iron ball.\n");
    set_data(SK_ATTACK_MORNING_STAR, ATMOD_MORNING_STAR,OBMOD_MORNING_STAR,8);
    add_lwvs(0, 2700, 675, 38);
    set_wield_size(2000, 2);
    set_value(1600);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_MORNING_STAR : 25]) );
}
