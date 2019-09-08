inherit "/base/container";
inherit "/base/misc/attach";
inherit "/base/misc/cook";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <slots.h>

static void
create_container()
{
    set_name("pot");
    add_adj("cooking");
    set_short("cooking pot");
    set_pshort("cooking pots");
    set_long("It's a fine cooking pot of iron.\n");
    set_lwvs(0,1500,400,15);
    set_value(70);
    set_max_weight(10000);
    set_max_volume(9500);
    add_prop(CONT_RIGID,1);
    add_prop(OBJ_MATERIAL, M_IRON);

    set_attach_slots(5); /* for the wood */
}

nomask int
attach_filter(object ob)
{
    if (E(this_object()) && living(E(this_object())))
	return 0;
    if (!function_object("query_burning", ob))
	return 0;
    return 1;
}
