inherit "/home/yirel/food";

#include <ingredients.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

#define HEAPFOOD 1
static void
create_food()
{
    set_name("egg");  
    add_adj("chicken");
    set_short("egg");
    set_pshort("eggs");
#if HEAPFOOD
    set_long("A chicken egg.\n");
#else
    set_long("A dodo egg.\n");
#endif
#if HEAPFOOD
    set_plong("Some chicken eggs.\n");
    set_heap_id(MASTER);
#endif
    set_amount(30);  /* food value */
    add_lwvs(0, 35, 300, 120);
    add_reload("reload_egg", "init_egg");
    set_rot(150,100);
    set_value(3);
    add_prop(OBJ_MATERIAL, M_FOOD);
    set_ingredient(INGREDIENT_EGG);
    set_waste_product("/home/yirel/chicken_eggshell");
}

void
init_egg(mixed *data)
{
    set_amount(data[0]);
    add_lwvs(0, data[1] - query_weight(),
             data[2] - query_volume(),
             data[3] - query_size());
}            

mixed *
reload_egg()
{
    return ({ query_amount(), query_weight(), query_volume(), query_size() });
}
