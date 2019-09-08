inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("glove");
    add_adj("right");
    add_adj("soft");
    add_adj("leather");
    add_adj("hobbit");
    set_short("glove");
    set_pshort("gloves");
    set_long("This is a right hand soft leather glove made for "+
	     "small hands.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_HAND+1 }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 245, 330, 4);
    set_value(65);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
