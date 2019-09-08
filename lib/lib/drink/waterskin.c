inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("waterskin");
    add_name("skin");
    add_adj("leather");
    set_short("waterskin");
    set_pshort("waterskins");
    set_long("A leather waterskin.\n");
    set_max_weight(150 + 2100);
    set_max_volume(100 + 2100);
    set_lwvs(0, 150, 100, 10);
    set_liquid_type(LIQ_WATER, "clear", "water");
    set_fill(2100);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    set_value(30);
}
