inherit "/base/container";
inherit "/system/parse";
inherit "/base/misc/openclose";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("chest");
    set_short("wooden chest");
    set_pshort("wooden chests");
    set_long("A small wooden chest.\n");
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_CLOSED, 1);
    set_lwvs(0, 5000, 5000, 40);
    set_max_weight(10000);
    set_max_volume(10000);
    set_value(10);
    add_prop(OBJ_MATERIAL, M_OAK);
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
