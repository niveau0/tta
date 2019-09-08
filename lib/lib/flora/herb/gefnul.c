inherit "/base/herb";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static void
create_herb()
{
    add_adj("bluish");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "lichen",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "lichens",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "large",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A slippery bluish lichen.\n",
		3 : "A large bluish lichen.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some slippery bluish lichens.\n",
		3 : "Some large bluish lichens.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "bluish lichen",
		3 : "large bluish lichen",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "bluish lichens",
		3 : "large bluish lichens",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 2 }),
		3 : ({ 10, 8, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 90000,
		3 : 95000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("gefnul");
    set_herb_pname("gefnuls");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the lichen to regain health.\n";
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
    tp->add_hp(100 * ret);
    return ret;
}
