inherit "/base/container";
inherit "/system/parse";
inherit "/base/misc/openclose";

#include <language.h>
#include <material.h>
#include <properties.h>
#include <slots.h>

static void
create_container()
{
    set_name("purse");
    set_short("leather purse");
    set_pshort("leather purses");
    set_long("A small leather purse for your money.\n");
    set_lwvs(0,10,25,4);
    set_value(10);
    set_max_weight(1500);
    set_max_volume(500);
    add_prop(OBJ_MATERIAL, M_LEATHER);
    add_prop(OBJ_ATTACH_SIZE, 1);
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
