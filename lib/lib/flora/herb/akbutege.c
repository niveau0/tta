inherit "/base/herb";

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
		2 : "A fresh red leaf.\n",
		3 : "A big red leaf.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh red leaves.\n",
		3 : "Some big red leaves.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "red leaf",
		3 : "big red leaf",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "red leaves",
		3 : "big red leaves",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 2 }),
		3 : ({ 10, 8, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 5,
		2 : 300,
		3 : 400,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("akbutege");
    set_herb_pname("akbutegi");
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
    tp->add_hp((random(10 * (query_grow_phase() - 1)) + 1) * ret);
    return ret;
}
