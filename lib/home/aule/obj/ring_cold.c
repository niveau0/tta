inherit "/base/object";
inherit "/base/misc/wear";

#include <event.h>
#include <living.h>
#include <material.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static nomask void
create_object()
{
    set_name("ring");
    set_pname("rings");
    set_short("ring with a white stone");
    set_pshort("rings with a white stone");
    set_long("A ring made of enchanted iron with a white stone in it. "+
	     "You\nfeel the power of magic from it.\n");
    set_lwvs(0, 2, 1, 1);
    set_layer(LAYER_CHAIN);
    set_locations( ({ SLT_HAND }) );
    set_value(100000);
    set_race_sizes(0, 1000);
    add_prop(OBJ_MATERIAL, M_ENCH_IRON);
    add_prop(IS_WEARABLE,1);
    add_event(E_WEAR, "object_worn", TO());
    add_event(E_REMOVE_TOOL, "object_removed", TO());
}

nomask void
object_worn(object tp, object ob)
{
    if (!tp || ob != TO())
	return;
    tp->add_tmp_skill(SK_RESIST_COLD, 30);
}

nomask void
object_removed(object tp, object ob)
{
    if (!tp || ob != TO())
	return;
    tp->add_tmp_skill(SK_RESIST_COLD, -30);
}
