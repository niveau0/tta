inherit "/base/herb";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_herb()
{
    add_adj("yellow");
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
		2 : "A fresh yellow fruit.\n",
		3 : "A big yellow fruit.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh yellow fruits.\n",
		3 : "Some big yellow fruits.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "yellow fruit",
		3 : "big yellow fruit",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "yellow fruits",
		3 : "big yellow fruits",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 25, 14, 2 }),
		3 : ({ 30, 25, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 5000,
		2 : 100000,
		3 : 110000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("winclamit");
    set_herb_pname("winclamits");
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
    tp->add_hp((random(298) + 3) * ret);
    return ret;
}
