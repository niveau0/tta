inherit "/base/armor";

#include <combat.h>
#include <material.h>
#include <race.h>
#include <slots.h>
#include <properties.h>

static void
create_armor()
{
    set_name("underwear");
    set_short("cotton underwear");
    set_pshort("cotton underwears");
    set_long("This is an underwear body, made of cotton.\n"+
	      "It has the size for a hobbit.\n");
    add_lwvs(0, 500, 200, 6);
    set_at(A_SKIN);
    set_layer(LAYER_UNDERWEAR);
    set_locations( ({ SLT_ABDOMEN , SLT_CHEST, SLT_ARM+1,
			  SLT_ARM+2, SLT_LEG+1, SLT_LEG+2 }) );
    set_value(50);
    set_race_sizes(HOBBIT_MIN, HOBBIT_MAX);
    add_prop(OBJ_MATERIAL, M_COTTON);
}
