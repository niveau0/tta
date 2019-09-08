inherit "/base/herb";

#include <event.h>
#include <effects.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

#define LEVEL 50

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
		1 : 10000,
		2 : 430000,
		3 : 440000,
		]),
	    PLANT_FOOD : ([
		1 : 1,
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("ul-naza");
    set_herb_pname("ul-nazi");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the leaf to remove any poison.\n";
}

int
eat_cmd()
{
    int     lev_poison, ret, i, j, sz;
    string  *files;
    object  tp;
    mapping poison;
    
    if ((ret = ::eat_cmd()) < 1)
	return ret;

    tp = this_player();

    if (!mappingp(poison = (tp)->query_effects()[POISON_EFFECT]))
	return ret;

    if (!(sz = sizeof(files = map_indices(poison) )))
	return ret;

    for(i=0; i<sz; i++)
    {
	lev_poison = files[i]->query_level();
	if(random(LEVEL) - random(lev_poison) > 0)
	{
	    j++;
	    files[i]->remove_poison(tp);
	}
    }

    if (j == i)
    {
	tp->catch_tell("You feel your body fully cleaned from poison.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be glad about something.\n" }),
		MSG_SEE);
    }
    else if (!j)
    {
	tp->catch_tell("Nothing seems to happen.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be disappointed.\n" }),
		MSG_SEE);
    }
    else
    {
	tp->catch_tell("You feel some of the poisons be neutralized. \n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be somehow glad.\n" }),
		MSG_SEE);
    }
    return ret;
}
