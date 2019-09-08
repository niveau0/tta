inherit "/base/container";

#include <properties.h>
#include <material.h>
 
static void
create_container()
{
    set_name("eggshell");
    add_name("shell");
    add_adj("brown");
    add_adj("broken");
    add_adj("half");
    set_short("half eggshell");
    set_pshort("half eggshells");
    set_long("This is one half of a brown eggshell which has been broken.\n");
    set_lwvs(0, 5, 20, 2);
    set_max_weight(15);
    set_max_volume(150);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_BONE);
    add_prop(CONT_RIGID, 1);
}
