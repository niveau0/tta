inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("vest");
    add_adj("cotton");
    add_adj("hobbit");
    add_adj("red");
    set_short("cotton vest");
    set_pshort("cotton vest");
    set_long_bs("This red cotton vest is probably made for small humanoids. " +
		"The two rows of silver buttons shine in the light.\n");
    set_at(A_SKIN);
    set_layer(LAYER_CHAIN);
    set_locations( ({ SLT_CHEST }) );
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_lwvs(0, 2000, 6000, 7);
    set_value(60);
    add_prop(OBJ_MATERIAL, M_COTTON);
}
