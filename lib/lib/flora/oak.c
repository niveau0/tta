inherit "/base/plant";

#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_plant()
{
    set_max_phase(4);

    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "acorn",
		2 : "tree",
		]),
	    PLANT_PNAME : ([
		1 : "acorns",
		2 : "trees",
		]),
	    PLANT_ADJ : ([
		2 : "oak",
		3 : ({ "large", "oak" }),
		4 : ({ "very large", "oak" }),
		]),
	    PLANT_LONG : ([
		1 : "A small acorn.\n",
		2 : "A small oak tree.\n",
		3 : "A large oak tree, still growing.\n",
		4 : "A very large oak tree, it really looks great.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small acorns.\n",
		2 : "Some small oak trees.\n",
		3 : "Some large oak trees, still growing.\n",
		4 : "Some very large oak trees, they really look great.\n",
		]),
	    PLANT_SHORT : ([
		1 : "acorn",
		2 : "oak tree",
		3 : "large oak tree",
		4 : "very large oak tree",
		]),
	    PLANT_PSHORT : ([
		1 : "acorns",
		2 : "oak trees",
		3 : "large oak trees",
		4 : "very large oak trees",
		]),
	    PLANT_WVS : ([
		1 : ({ 5, 5, 1 }),
		2 : ({ 4380, 500, 100 }),
		3 : ({ 15470, 1000, 400 }),
		4 : ({ 32500, 2000, 1000 }),
		]),
	    PLANT_VALUE : ([
		1 : 1,
		2 : 10,
		3 : 20,
		4 : 30,
		]),
	    ]));
    add_prop(OBJ_MATERIAL, M_OAK);
}
