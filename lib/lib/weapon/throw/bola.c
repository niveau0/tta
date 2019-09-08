inherit "/base/weapon";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_weapon()
{
    set_name("bola");
    set_short("bola");
    set_pshort("bolas");
    set_long("A fine worked bola with three iron balls.\n");
    set_data(SK_ATTACK_BOLA, ATMOD_BOLA, OBMOD_BOLA, 7);
    add_lwvs(0, 1800, 450, 53);
    set_wield_size(800, 1);
    set_value(500);
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(SKILL_SUPPORT, ([SK_ATTACK_BOLA : 25]) );
}

int
query_throw_skill()
{
    return SK_ATTACK_BOLA;
}
