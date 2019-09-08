inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("scimitar");
    add_name("blade");
    set_short("scimitar");
    set_pshort("scimitars");
    set_long("A sharp iron scimitar.\n");
    set_data(SK_ATTACK_SCIMITAR, ATMOD_SCIMITAR, OBMOD_SCIMITAR, 4);
    add_lwvs(0, 1850, 463, 38);
    set_wield_size(1500, 2);
    set_value(1000);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_SCIMITAR : 25]) );
}

