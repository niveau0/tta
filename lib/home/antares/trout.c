inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("trout");
    add_name("flappy");
    set_short("trout");
    set_pshort("trouts");
    set_long("A large trout.\n");
    set_data(SK_ATTACK_DAGGER, ATMOD_DAGGER, OBMOD_DAGGER, 1);
    add_lwvs(0, 520, 130, 15);
    set_wield_size(500, 2);
    set_value(300);
    add_prop(OBJ_MATERIAL, M_STEEL);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_DAGGER : 25]) );
}

int
query_throw_skill()
{
    return SK_ATTACK_DAGGER_THR;
}
