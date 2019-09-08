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
		2 : "cactus",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "cacti",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry green cactus.\n",
		3 : "A big green cactus.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry green cacti.\n",
		3 : "Some big green cacti.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "green cactus",
		3 : "big green cactus",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "green cacti",
		3 : "big green cacti",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 100, 80, 4 }),
		3 : ({ 200, 180, 8 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 55000,
		3 : 60000,
		]),
	    PLANT_FOOD : ([
		2 : 50,
		3 : 100,
		]),
	    ]));
    set_herb_name("gariig");
    set_herb_pname("gariigs");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the cactus to regain health.\n";
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
    tp->add_hp(30 * ret);
    return ret;
}
