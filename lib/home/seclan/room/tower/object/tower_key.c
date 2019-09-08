inherit "/base/object";

#include <material.h>
#include <properties.h>

void
create_object()
{
    set_name("symbol");
    add_adj("metal");
    set_short("metal symbol");
    set_pshort("metal symbols");
    set_long("The symbol forms a complex rune. "+
	     "It is made from a black metal.\n");
    set_lwvs(0, 150, 350, 3);
    add_prop(OBJ_MATERIAL, M_BLACK_ALLOY);
    set_value(5);
}
