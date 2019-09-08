inherit "/base/herb";

#include <event.h>
#include <effects.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

#define LEVEL 3

static string *affected_poisons;

static void
create_herb()
{
    add_adj("red");
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
		2 : "A fresh red berry.\n",
		3 : "A big red berry.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh red berries.\n",
		3 : "Some big red berries.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "red berry",
		3 : "big red berry",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "red berries",
		3 : "big red berries",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 30, 20, 2 }),
		3 : ({ 40, 30, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 500,
		2 : 30000,
		3 : 40000,
		]),
	    PLANT_FOOD : ([
		2 : 20,
		3 : 30,
		]),
	    ]));
    set_herb_name("mook");
    set_herb_pname("mooks");
    set_heap_id(MASTER);
    affected_poisons = ({"/lib/poison/galenaana",
			 "/lib/poison/hith_i_girith",
			 "/lib/poison/jegga",
			 "/lib/poison/jitsu",
			 "/lib/poison/jitsutyr",
			 "/lib/poison/kly",
			 "/lib/poison/uraana",
			 "/lib/poison/vuraana" });
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Eat the berry to remove respiratory poisons.\n";
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
