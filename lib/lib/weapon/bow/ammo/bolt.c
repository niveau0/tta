inherit "/base/missile";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <weapon.h>

static void
create_missile()
{
    set_name("bolt");
    add_adj("wooden");
    set_short("wooden bolt");
    set_pshort("wooden bolts");
    set_long("A finely crafted wooden bolt with a metal point.\n");
    set_plong("Some finely crafted wooden bolts with metal points.\n");
    set_missile_type(MISSILE_BOLT);
    set_heap_size(1);
    set_heap_id(MASTER);
    set_value(1);
    add_lwvs(0, 50, 20, 8);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(IS_AMMO, 1);
}
