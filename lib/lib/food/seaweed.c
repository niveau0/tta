inherit "/base/food";

#include <ingredients.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_food()
{
    set_name("piece of seaweed");
    set_name("seaweed");
    set_name("weed");
    set_short("piece of seaweed");
    set_pshort("pieces of seaweed");
    set_long("It's some seaweed. It doesn't smell too good, but you could "+
	     "eat it.\n");
    set_plong("It's some seaweed. It doesn't smell too good, but you could "+
	      "eat it.\n");
    set_heap_id(MASTER);
    set_value(70);
    set_amount(15);
    add_lwvs(0, 40, 20, 2);
    add_prop(OBJ_MATERIAL, M_FOOD);
    set_ingredient(INGREDIENT_SEAWEED);
}
