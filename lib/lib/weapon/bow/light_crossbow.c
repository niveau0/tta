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
    set_name("crossbow");
    add_adj("light");
    set_short("light crossbow");
    set_pshort("light crossbows");
    set_long("A finely crafted light crossbow.\n");
    /* melee attacks with the bow */
    set_data(SK_ATTACK_CLUB, ATMOD_CLUB,
	     OBMOD_CLUB, 4);
    set_missile_data(SK_ATTACK_LIGHT_CROSSBOW, ATMOD_LIGHT_CROSSBOW,
		     OBMOD_LIGHT_CROSSBOW, 4);
    add_lwvs(0, 1500, 500, 29); /* values may need to be checked */
    set_wield_size(2000, -1);
    set_missile_type(MISSILE_BOLT);
    set_weapon_range(4000);
    set_value(1900);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_LIGHT_CROSSBOW : 25]) );
}
