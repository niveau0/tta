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
    add_adj("white");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "clove",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "cloves",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A white clove.\n",
		3 : "A big white clove.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some white cloves.\n",
		3 : "Some big white cloves.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "white clove",
		3 : "big white clove",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "white cloves",
		3 : "big white cloves",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 15, 10, 2 }),
		3 : ({ 20, 15, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 20,
		2 : 200,
		3 : 300,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("thurl");
    set_herb_pname("thurl");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_THURL);
}

string
query_usage()
{
    return "Brew the clove to get a potion of healing.\n";
}
