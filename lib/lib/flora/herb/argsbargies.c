inherit "/base/herb";

#include <effects.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

#define LEVEL 4

static string *affected_poisons;

static void
create_herb()
{
    add_adj("yellow");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "flower",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "flowers",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "small",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A yellow flower.\n",
		3 : "A big yellow flower.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some yellow flowers.\n",
		3 : "Some big yellow flowers.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "yellow flower",
		3 : "big yellow flower",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "yellow flowers",
		3 : "big yellow flowers",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 20, 15, 10 }),
		3 : ({ 30, 25, 15 }),
		]),
	    PLANT_VALUE : ([
		1 : 1000,
		2 : 38000,
		3 : 40000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("argsbargies");
    set_herb_pname("argsbargies");
    set_heap_id(MASTER);
    affected_poisons = ({"/lib/poison/dynallca",
			 "/lib/poison/kaktu",
			 "/lib/poison/trusa" });
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the flower to remove muscle poisons.\n";
}

int
eat_cmd()
{
    int     lev_poison, ret, i, sz;
    string  *files;
    object  tp;
    mapping poison;
    
    if ((ret = ::eat_cmd()) < 1)
	return ret;

    tp = this_player();
   
    if (!mappingp(poison = tp->query_effects()[POISON_EFFECT]))
	return ret;

    if (!(sz = sizeof(files = map_indices(poison))))
	return ret;

    for(i=0; i<sz; i++)
    {
	if (IN_ARRAY(files[i],affected_poisons))
	    break;
    }

    lev_poison = files[i]->query_level();
    if ( (i == sz) || (random(LEVEL + query_grow_phase() - 2) -
		       random(lev_poison) <= 0) )
    {
	tp->catch_tell("Nothing seems to happen.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " looks disappointed.\n" }),
		MSG_SEE);
    }

    else
    {
	files[i]->remove_poison(tp);
	tp->catch_tell("You feel your body cleaned from the poison.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be glad about something.\n" }),
		MSG_SEE);
    }

    return ret;
}
