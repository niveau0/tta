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
		2 : "root",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "roots",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry red root.\n",
		3 : "A big red root.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry red roots.\n",
		3 : "Some big red roots.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "red root",
		3 : "big red root",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "red roots",
		3 : "big red roots",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 40, 30, 5 }),
		3 : ({ 70, 60, 10 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 49000,
		3 : 55000,
		]),
	    PLANT_FOOD : ([
		2 : 10,
		3 : 30,
		]),
	    ]));
    set_herb_name("quilmufur");
    set_herb_pname("quilmufir");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_QUILMUFUR);
}

string
query_usage()
{
    return "Brew the root to get a potion of remove conversion poisons.\n";
}
