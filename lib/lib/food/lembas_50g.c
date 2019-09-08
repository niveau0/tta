inherit "/base/food";

#include <properties.h>
#include <macros.h>
#include <material.h>

static void
create_food()
{
    set_name("lembas");
    set_short("lembas");
    set_pshort("lembas");
    set_long("This is some fine bread, made by elves.\n");
    set_plong("These are some fine breads, made by elves.\n");
    set_heap_id(MASTER);
    set_amount(1000); /* gram */
    add_lwvs(0, 51, 40, 1);
    set_value(250);
    add_prop(OBJ_MATERIAL, M_FOOD); 
}

int
eat_cmd()
{
    int bla;
	 
    if ((bla = ::eat_cmd()) == -1)
	return -1;

    this_player()->add_hp(2);
    this_player()->add_mana(3);
    this_player()->add_fatigue(random(30)+20);
    this_player()->catch_tell("You feel somehow refreshed.\n");

    return bla;
}
