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
		2 : "nut",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "nuts",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry brown nut.\n",
		3 : "A big brown nut.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry brown nuts.\n",
		3 : "Some big brown nuts.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "brown nut",
		3 : "big brown nut",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "brown nuts",
		3 : "big brown nuts",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 1 }),
		3 : ({ 10, 8, 2 }),
		]),
	    PLANT_VALUE : ([
		1 : 500,
		2 : 50000,
		3 : 55000,
		]),
	    PLANT_FOOD : ([
		2 : 15,
		3 : 20,
		]),
	    ]));
    set_herb_name("fek");
    set_herb_pname("feks");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_FEK);
}

string
query_usage()
{
    return "Brew the nut to get a potion that stops your bleeding.\n";
}
