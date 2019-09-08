inherit "/base/container";
inherit "/base/misc/openclose";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("oven");
    set_short("iron oven");
    set_pshort("iron ovens");
    set_long_bs("This oven is worked from dark metal. An opening on its "+
		"top shows where to apply a pipe for the emitted smoke. "+
		"On its front is a wide door with several small slits in "+
		"it. Attached at the ovens bottom are four metalfeet.\n");
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_CLOSED, 1);
    set_lwvs(0, 20000, 150000, 40);
    set_max_weight(200000);
    set_max_volume(100000);
    set_value(70);
    add_prop(OBJ_MATERIAL, M_IRON);
}

/*
 * Function name: short
 * Description:   gives modified short description 
 * Returns:       string
 */
string
short()
{
    if (!query_prop(CONT_CLOSED))
	return "open " + ::short();
    return "closed " + ::short();
}

/*
 * Function name: query_burning
 * Description:   is true if object can be used to light something,
 *                here: always
 * Returns:       integer
 */
nomask int
query_burning()
{
    return 1;
}
