inherit "/base/herb";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_herb()
{
    add_adj("blue");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "berry",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "berries",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A fresh blue berry.\n",
		3 : "A big blue berry.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh blue berries.\n",
		3 : "Some big blue berries.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "blue berry",
		3 : "big blue berry",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "blue berries",
		3 : "big blue berries",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 2 }),
		3 : ({ 10, 8, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 500,
		2 : 10000,
		3 : 11000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("mirenna");
    set_herb_pname("mirenni");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the berry to regain health.\n";
}

int
eat_cmd()
{
    int    ret;
    object tp;
    
    if ((ret = ::eat_cmd()) < 1)
	return ret;

    tp = this_player();
    tp->catch_tell("You feel freshed and vitality flows through your body.\n");
    tp->add_hp(10 * ret);
    return ret;
}
