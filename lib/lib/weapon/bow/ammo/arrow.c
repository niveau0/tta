inherit "/base/missile";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <weapon.h>

static void
create_missile()
{
    set_name("arrow");
    add_adj("wooden");
    set_short("wooden arrow");
    set_pshort("wooden arrows");
    set_long("A fine made wooden arrow with a metal point.\n");
    set_plong("Some fine made wooden arrows with metal points.\n");
    set_missile_type(MISSILE_ARROW);
    set_heap_size(1);
    set_heap_id(MASTER);
    set_value(1);
    add_lwvs(0, 50, 20, 23);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(IS_AMMO, 1);
}
