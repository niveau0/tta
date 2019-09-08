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
		2 : "reed",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "reeds",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "short",
		3 : "long",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry brown reed.\n",
		3 : "A long brown reed.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry brown reeds.\n",
		3 : "Some long brown reeds.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "brown reed",
		3 : "long brown reed",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "brown reeds",
		3 : "long brown reeds",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 20, 15, 7 }),
		3 : ({ 30, 20, 10 }),
		]),
	    PLANT_VALUE : ([
		1 : 2500,
		2 : 160000,
		3 : 170000,
		]),
	    PLANT_FOOD : ([
		2 : 10,
		3 : 20,
		]),
	    ]));
    set_herb_name("baalak");
    set_herb_pname("baalaks");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_BAALAK);
}

string
query_usage()
{
    return "Brew the reed to get a potion that repairs shattered bones.\n";
}
