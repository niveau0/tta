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
    add_adj("red");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "moss",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "moss",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A slippery red moss.\n",
		3 : "A big wet red moss.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some slippery red moss.\n",
		3 : "Some big wet red moss.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "red moss",
		3 : "big red moss",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "red moss",
		3 : "big red moss",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 10, 8, 2 }),
		3 : ({ 20, 18, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 75000,
		3 : 80000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("reglen");
    set_herb_pname("reglen");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_REGLEN);
}

string
query_usage()
{
    return "Brew the moss to get a potion of healing.\n";
}
