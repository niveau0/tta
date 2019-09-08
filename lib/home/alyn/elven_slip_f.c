inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <race.h>
#include <slots.h>
#include <properties.h>

static void
create_armor()
{
    set_name("slip");
    set_short("cotton slip");
    set_pshort("cotton slip");
    set_long_bs("This is a cotton slip made for female humanoids.\n");
    add_lwvs(0,60,80,2);
    set_at(A_SKIN);
    set_layer(LAYER_UNDERWEAR);
    set_locations( ({ SLT_ABDOMEN }) );
    set_value(30);
    set_race_sizes(ELF_MIN, ELF_MAX);
    add_prop(OBJ_MATERIAL, M_COTTON);
}
