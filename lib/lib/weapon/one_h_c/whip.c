inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("whip");
    set_short("whip");
    set_pshort("whips");
    set_long("A long whip made of soft leather with a wooden handle.\n");
    set_data(SK_ATTACK_WHIP, ATMOD_WHIP, OBMOD_WHIP, 6);
    add_lwvs(0, 1600, 457, 108);
    set_wield_size(1300, 1);
    set_value(200);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_WHIP : 25]) );
}
