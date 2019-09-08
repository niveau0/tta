inherit "/base/object";

#include <material.h>
#include <properties.h>

static void
create_object()
{
    set_name("wick");
    add_adj("white");
    add_adj("linen");
    set_short("linen wick");
    set_pshort("linen wicks");
    set_long("A white linen wick for oil lamps.\n");
    set_value(1);
    add_lwvs(0, 5, 10, 20);
    add_prop(OBJ_MATERIAL, M_LINEN); 
}

/*
 * Function name: query_wick
 * Description:   proof this is a wick
 * Returns:       1 
 */
int
query_wick()
{
    return 1;
}
