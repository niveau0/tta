inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <properties.h>
#include <race.h>
#include <slots.h>

static void
create_armor()
{
    set_name("skirt");
    add_adj("leather");
    add_adj("elven");
    add_adj("short");
    add_adj("black");
    set_short("short black leather skirt");
    set_pshort("short black leather skirts");
    set_long_bs("A simple black leather skirt made for female humanoids.\n" +
	        "It is quite short and would barely cover the thighs.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_CLOTH);
    set_locations( ({ SLT_ABDOMEN }) );
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_lwvs(0, 2800, 4000, 8);
    set_value(80);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
