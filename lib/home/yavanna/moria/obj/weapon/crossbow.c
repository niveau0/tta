inherit "/base/missile_weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>
#include <weapon.h>

static void
create_weapon()
{
    set_name("crossbow");
    add_adj("heavy");
    set_short("heavy crossbow");
    set_pshort("heavy crossbows");
    set_long("A fine crafted crossbow with high tension.\n"+
	     "Looks like dwarvish work");
    /* melee attacks with the bow */
    set_data(SK_ATTACK_QUARTERSTAFF, ATMOD_QUARTERSTAFF,
	     OBMOD_QUARTERSTAFF, 4);
    set_missile_data(SK_ATTACK_HEAVY_CROSSBOW, ATMOD_HEAVY_CROSSBOW,
		     OBMOD_HEAVY_CROSSBOW, 4);
    add_lwvs(0, 1150, 329, 53);
    set_wield_size(2700, -2);
    set_missile_type(MISSILE_BOLT);
    set_weapon_range(4800);
    set_value(1700);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_HEAVY_CROSSBOW : 35]) );
}
