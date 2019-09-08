inherit "/base/object";

#include <material.h>
#include <properties.h>

void
create_object()
{
    set_name("statue");
    set_short("big marble statue");
    set_pshort("big marble statues");
    set_long(" This is a big marble statue of the famous Nase. It shows the " +
	     "victim of the\ngods as he mostly was - poisoned, diseased " +
	     "and with broken bones.\n All poison and evil magic had " +
	     "been proofed on him. He was the most pityful\nperson that " +
	     "ever lived on Arda.\n");
    add_lwvs(0, 50000, 100000, 250);
    set_value(60);
    add_prop(OBJ_MATERIAL, M_STONE);
}
