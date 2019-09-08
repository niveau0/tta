inherit "/base/herb";

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
		2 : "leaf",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "leaves",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A fresh green leaf.\n",
		3 : "A big green leaf.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh green leaves.\n",
		3 : "Some big green leaves.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "green leaf",
		3 : "big green leaf",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "green leaves",
		3 : "big green leaves",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 2 }),
		3 : ({ 10, 8, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 500,
		2 : 30000,
		3 : 40000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("cusamar");
    set_herb_pname("cusamari");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the leaf to regain health.\n";
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
    tp->add_hp( ((random(10) + 1) * (5 + query_grow_phase()) + 10) * ret);
    return ret;
}
