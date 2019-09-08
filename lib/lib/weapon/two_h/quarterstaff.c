inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("quarterstaff");
    add_name("staff");
    add_adj("wooden");
    set_short("quarterstaff");
    set_pshort("quarterstaffs");
    set_long("A long wooden quarterstaff of good quality.\n");
    set_data(SK_ATTACK_QUARTERSTAFF, ATMOD_QUARTERSTAFF,
	     OBMOD_QUARTERSTAFF, 3);
    add_lwvs(0, 1850, 529, 71);
    set_wield_size(2500, 2);
    set_value(5);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_QUARTERSTAFF : 25]) );
}
