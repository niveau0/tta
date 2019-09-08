inherit "/base/herb";

#include <event.h>
#include <effects.h>
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
		2 : 27000,
		3 : 30000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("shen");
    set_herb_pname("shen");
    set_heap_id(MASTER);
    affected_poisons = ({"/lib/poison/acaana",
			 "/lib/poison/asgurash",
			 "/lib/poison/cathaana",
			 "/lib/poison/juth",
			 "/lib/poison/ruth_i_iaur",
			 "/lib/poison/slird",
			 "/lib/poison/ul_acaana",
			 "/lib/poison/wek_baas" });
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the leaf to remove nerve poisons.\n";
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

    if (!mappingp(poison = (tp)->query_effects()[POISON_EFFECT]))
	return ret;

    if (!(sz = sizeof(files = map_indices(poison))))
	return ret;

    for(i=0;i<sz;i++)
    {
	if(IN_ARRAY(files[i],affected_poisons))
	    break;
    }

    lev_poison = files[i]->query_level(tp);

    if ( (i == sz) || (random(LEVEL + query_grow_phase() - 2) -
		       random(lev_poison) <= 0) )
    {
	tp->catch_tell("Nothing seems to happen.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be disappointed.\n" }),
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
