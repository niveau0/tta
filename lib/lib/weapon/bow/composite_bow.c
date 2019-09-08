inherit "/base/missile_weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>
#include <weapon.h>

static void
create_missile_weapon()
{
    set_name("bow");
    add_adj("composite");
    set_short("composite bow");
    set_pshort("composite bows");
    set_long("A fine crafted wooden bow with high tension.\n");
    /* melee attacks with the bow */
    set_data(SK_ATTACK_QUARTERSTAFF, ATMOD_QUARTERSTAFF,
	     OBMOD_QUARTERSTAFF, 4);
    set_missile_data(SK_ATTACK_COMPOSITEBOW, ATMOD_COMPOSITEBOW,
		     OBMOD_COMPOSITEBOW, 4);
    add_lwvs(0, 1150, 329, 53);
    set_wield_size(2700, -2);
    set_missile_type(MISSILE_ARROW);
    set_weapon_range(4800);
    set_value(1700);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_COMPOSITEBOW : 25]) );
}
