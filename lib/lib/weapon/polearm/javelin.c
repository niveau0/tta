inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("javelin");
    add_name("spear");
    set_short("javelin");
    set_pshort("javelins");
    set_long("A long wooden javelin with an iron point.\n");
    set_data(SK_ATTACK_JAVELIN, ATMOD_JAVELIN, OBMOD_JAVELIN, 4);
    add_lwvs(0, 1850, 529, 65);
    set_wield_size(1500, 2);
    set_value(300);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_JAVELIN : 25]) );
}

int
query_throw_skill()
{
    return SK_ATTACK_JAVELIN_THR;
}
