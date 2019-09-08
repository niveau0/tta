inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("tinder&stone");
    add_name("tinder and stone");
    add_name("tinderbox");
    add_name("box");
    add_name("tinder");
    set_short("tinderbox");
    set_pshort("tinderboxes");
    set_long_bs("This is a little box with some tinder and stones in it. "+
		"It is needed to start a fire or light something.\n");
    add_prop(OBJ_MATERIAL, M_STONE);
    add_lwvs(0, 30, 20, 5);
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
