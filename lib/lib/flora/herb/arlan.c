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
		2 : "A fresh yellow leaf.\n",
		3 : "A big yellow leaf.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some fresh yellow leaves.\n",
		3 : "Some big yellow leaves.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "yellow leaf",
		3 : "big yellow leaf",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "yellow leaves",
		3 : "big yellow leaves",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 5, 4, 2 }),
		3 : ({ 10, 8, 3 }),
		]),
	    PLANT_VALUE : ([
		1 : 50,
		2 : 1300,
		3 : 2000,
		]),
	    PLANT_FOOD : ([
		2 : 5,
		3 : 10,
		]),
	    ]));
    set_herb_name("arlan");
    set_herb_pname("arlans");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

string
query_usage()
{
    return "Use it on someone, to regain health again.\n";
}

int
use_cmd(string name)
{
    object tp, ob, herb, here, *obj;

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
    tp->catch_tell("You rub the leaf on "+ ob->query_possessive() + 
		   " skin.\n");
    if (tp != ob)
    {
	ob->catch_msg(({ QCTNAME(tp), " rubs ", QNAME(this_object()),
			 " on your skin.\n" }), 0, 0);
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " rubs ", QNAME(this_object()),
			" on ", QTNAME(ob), "'s skin.\n" }),
		     MSG_SEE, tp);
    }
    else
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " rubs ", QNAME(this_object()),
			" on " + tp->query_possessive() + " skin.\n" }),
		     MSG_SEE, tp);
    move(E(ob)); /* avoid second usage */
    add_prop(OBJ_NO_SHOW, 1);
    call_out("do_heal", 3, ob);
}

static void
do_heal(object tp)
{
    if (tp)
    {
	tp->add_hp(random(6)+4);
	tp->catch_tell("You feel freshed and vitality flows " +
		       "through your body.\n");
    }
    remove_object();
}
