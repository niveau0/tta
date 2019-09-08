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
    add_adj("green");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "stalk",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "stalks",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "long",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A fresh green stalk.\n",
		3 : "A long green stalk.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh green stalks.\n",
		3 : "Some long green stalks.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "green stalk",
		3 : "long green stalk",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "green stalks",
		3 : "long green stalks",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 35, 30, 10 }),
		3 : ({ 50, 40, 15 }),
		]),
	    PLANT_VALUE : ([
		1 : 1500,
		2 : 110000,
		3 : 120000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("bursthelas");
    set_herb_pname("bursthelas");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_BURSTHELAS);
}

string
query_usage()
{
    return "Brew the stalk to get a potion that repairs shattered bones.\n";
}
