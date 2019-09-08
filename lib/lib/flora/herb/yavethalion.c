inherit "/base/herb";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_herb()
{
    add_adj("reddish");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "fruit",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "fruits",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A fresh reddish fruit.\n",
		3 : "A big reddish fruit.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh reddish fruits.\n",
		3 : "Some big reddish fruits.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "reddish fruit",
		3 : "big reddish fruit",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "reddish fruits",
		3 : "big reddish fruits",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 15, 10, 2 }),
		3 : ({ 26, 18, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 50000,
		3 : 55000,
		]),
	    PLANT_FOOD : ([
		2 : 10,
		3 : 20,
		]),
	    ]));
    set_herb_name("yavethalion");
    set_herb_pname("yavethalions");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the fruit to regain health.\n";
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
    tp->add_hp((random(46) + 5) * ret);
    return ret;
}
