inherit "/base/herb";

#include <event.h>
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
		2 : ({ 20, 10, 2 }),
		3 : ({ 30, 20, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 5000,
		2 : 180000,
		3 : 190000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("hugburtun");
    set_herb_pname("hugburtin");
    set_value(180000);
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Use it on someone, to care for the bleeding wounds.\n";
}

int
use_cmd(string name)
{
    int     *ind, sz, i;
    object  tp, ob, here, *obj;
    mapping wounds;

    tp = this_player();
    if (!query_analysed(tp))
    {
	tp->catch_tell("You don't know how to use it.\n");
	return -1;
    }
    if (!name)
    {
	tp->catch_tell(query_usage());
	return -1;
    }
    obj = tp->visible(I((here = E(tp)), 2));
    if (!sizeof(obj = parse(name, obj, tp)))
    {
	tp->catch_tell("You cannot see anyone called '" + name + "'.\n");
	return -1;
    }
    ob = obj[0];

    if (!ob->query_bloodloss())
    {
	tp->catch_tell("There is no bleeding.\n");
	return -1;
    }
    tp->catch_tell("You put the fruit on the wounds.\n");
    if (tp != ob)
    {
	ob->catch_msg(({ QCTNAME(tp), " puts ", QNAME(this_object()),
			 " on your wounds.\n" }), 0, 0);
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " puts ", QNAME(this_object()),
			" on ", QTNAME(ob), "'s wounds.\n" }),
		     MSG_SEE, tp);
    }
    else
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " puts ", QNAME(this_object()),
			" on " + tp->query_possessive() + " wounds.\n" }),
		     MSG_SEE, tp);
    set_heap_size(query_heap_size() - 1);

    sz = map_sizeof(wounds = ob->query_bloodwounds());
    ind = map_indices(wounds);

    for(i=0; i<sz; i++)
        ob->add_bloodloss(-wounds[ind[i]], ind[i]);

    return 1;
}
