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
		2 : "cone",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "cones",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry brown cone.\n",
		3 : "A big brown cone.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry brown cones.\n",
		3 : "Some big brown cones.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "brown cone",
		3 : "big brown cone",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "brown cones",
		3 : "big brown cones",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 10, 8, 2 }),
		3 : ({ 20, 17, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 65000,
		3 : 70000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("menelar");
    set_herb_pname("menelars");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
    set_ingredient(INGREDIENT_MENELAR);
}

string
query_usage()
{
    return "Brew the cone to get a potion of remove circulatory poisons.\n";
}
