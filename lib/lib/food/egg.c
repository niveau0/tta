inherit "/base/food";

#include <ingredients.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_food()
{
    set_name("egg");  
    add_adj("chicken");
    set_short("egg");
    set_pshort("eggs");
    set_long("A chicken egg.\n");
    set_plong("Some chicken eggs.\n");
    set_heap_id(MASTER);
    set_amount(30);  /* food value */
    add_lwvs(0, 35, 300, 120);
    set_value(3);
    add_prop(OBJ_MATERIAL, M_FOOD);
    set_ingredient(INGREDIENT_EGG);
}
