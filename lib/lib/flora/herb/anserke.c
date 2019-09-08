inherit "/base/herb";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <plant.h>
#include <properties.h>

static int    flowstop, *last_co;
static object applied;

static void
create_herb()
{
    add_adj("brown");
    set_max_phase(3);
    set_plant_data(
	([
	    PLANT_NAME : ([
		1 : "seed",
		2 : "root",
		]),
	    PLANT_PNAME : ([
		1 : "seeds",
		2 : "roots",
		]),
	    PLANT_ADJ : ([
		1 : "small",
		2 : "dry",
		3 : "big",
		]),
	    PLANT_LONG : ([
		1 : "A small seed.\n",
		2 : "A dry ugly brown root.\n",
		3 : "A big dry brown root.\n",
		]),
	    PLANT_PLONG : ([
		1 : "Some small seeds.\n",
		2 : "Some dry ugly roots.\n",
		3 : "Some big dry ugly roots.\n",
		]),
	    PLANT_SHORT : ([
		1 : "seed",
		2 : "brown root",
		3 : "big brown root",
		]),
	    PLANT_PSHORT : ([
		1 : "seeds",
		2 : "brown roots",
		3 : "big brown roots",
		]),
	    PLANT_WVS : ([
		1 : ({ 1, 1, 1 }),
		2 : ({ 25, 20, 4 }),
		3 : ({ 35, 30, 6 }),
		]),
	    PLANT_VALUE : ([
		1 : 2500,
		2 : 75000,
		3 : 80000,
		]),
	    PLANT_FOOD : ([
		2 : 10,
		3 : 20,
		]),
	    ]));
    set_herb_name("anserke");
    set_herb_pname("anserkes");
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_OTHER);
}

nomask string
query_usage()
{
    return "Use it on someone to care for the bleeding wounds.\n";
}

nomask int
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
    if (!ob->query_bloodloss())
    {
	tp->catch_tell("There is no bleeding wound.\n");
	return -1;
    }
    tp->catch_tell("You put the root on the wound.\n");
    if (tp != ob)
    {
	ob->catch_msg(({ QCTNAME(tp), " puts ", QNAME(this_object()),
			 " on your wound.\n" }), 0, 0);
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " puts ", QNAME(this_object()),
			" on ", QTNAME(ob), "'s wound.\n" }),
		     MSG_SEE, tp);
    }
    else
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " puts ", QNAME(this_object()),
			" on " + tp->query_possessive() + " wound.\n" }),
		     MSG_SEE, tp);
    herb = clone_object(MASTER);
    herb->set_applied_to(ob, query_status());
    herb->move(ob);
    set_heap_size(query_heap_size() - 1);
    return 1;
}

nomask void
set_applied_to(object ob, mapping st)
{
    applied = ob;
    set_heap_id(0); /* should be no heap anymore */
    set_status(st);
    call_out("start_flowstop", 15, ob);
}

static nomask void
start_flowstop(object tp)
{
    int fs, bl;
    
    if (!tp)
    {
	remove_object();
	return;
    }
    if ((fs = tp->query_prop(LIVE_BLOODFLOW_STOP)) <
	(bl = tp->query_bloodloss()))
    {
	flowstop = bl - fs;
	tp->change_int_prop(LIVE_BLOODFLOW_STOP, flowstop);
	add_event(E_MOVE, "living_move", tp);
	last_co = tp->query_coordinates();
    }
}

/* called if the patient moves */
nomask void
living_move(object ob, object from, object to)
{
    if (!applied || !distance(applied->query_coordinates(), last_co, 10))
    {
	if (applied)
	    applied->catch_msg(({ QCTNAME(this_object()),
				      " dissolves to dust...\n" }),
			       this_object(), MSG_SEE);
	call_out("remove_object", 0);
	return;
    }
}

varargs void
remove_object(object ob)
{
    if ((!ob || ob == this_object()) && applied && flowstop > 0)
	applied->change_int_prop(LIVE_BLOODFLOW_STOP, flowstop);
    ::remove_object(ob);
}
