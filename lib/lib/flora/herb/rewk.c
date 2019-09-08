inherit "/base/herb";

#include <ingredients.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_herb()
{
    add_adj("brown");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "nodule",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "nodules",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A fresh brown nodule.\n",
		3 : "A big brown nodule.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh brown nodules.\n",
		3 : "Some big brown nodules.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "brown nodule",
		3 : "big brown nodule",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "brown nodules",
		3 : "big brown nodules",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 10, 8, 2 }),
		3 : ({ 20, 16, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 50,
		2 : 900,
		3 : 1000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("rewk");
    set_herb_pname("rewks");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_REWK);
}

string
query_usage()
{
    return "Brew the nodule to get a potion of healing.\n";
}
