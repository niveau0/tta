inherit "plant";
inherit "/system/parse";

#include <analyse.h>
#include <macros.h>
#include <skills.h>
#include <properties.h>

static nomask void set_herb_name(string n);
static nomask void set_herb_pname(string n);

private string  herb_name,  /* analysed name of the herb */
                herb_pname; /* analysed plural name of the herb */
	      
static void
create_herb()
{
}

static nomask void
create_plant()
{
    create_herb();
    add_prop(IS_HERB, 1);
    set_heap_size(1);
}

string
query_usage()
{
    return "It's useless.\n";
}

nomask int 
analyse_cmd(object tp) /* this overrides /base/food */
{
    int state; 

    if (!tp)
	return -1;
    state = query_analysed(tp);

    if (state == ITEM_TRIED)
    {
	tp->catch_tell("You already tried to analyse this kind.\n");
	return -1;
    }
    if (state == ITEM_ANALYSED ||
	tp->skill_check(SK_HERB_LORE, 0, 0) > 0)
    {
	set_analysed(tp, ITEM_ANALYSED);
	heap_search_equal(E(this_object()));
	tp->catch_tell("You find out: It's " + capitalize(herb_name) + ".\n" +
		       query_usage());
	::analyse_cmd(tp); /* /base/food, herb might be poisoned */
	return 1;
    }
    set_analysed(tp, ITEM_TRIED);
    tp->catch_tell("You fail to analyse the " + short() + ".\n");
    return -1;
}

nomask string
query_heap_id()
{
    if (query_analysed(this_player()) == ITEM_ANALYSED)
	return herb_name + ::query_heap_id();
    return ::query_heap_id();
}

static nomask void
set_herb_name(string n)
{
    herb_name = n;
}

nomask string
query_herb_name()
{
    return herb_name;
}

static nomask void
set_herb_pname(string n)
{
    herb_pname = n;
}

nomask string
query_herb_pname()
{
    return herb_pname;
}

nomask string *
query_names()
{
    if (query_analysed(this_player()) == ITEM_ANALYSED)
	return ({ herb_name }) + ::query_names();
    return ::query_names();
}

nomask int
parse_object(string *cmd, int type, int *co, int d, object tp)
{
    int    i;
    string name, rname, rpname;
    
    if (!tp || query_analysed(tp) != ITEM_ANALYSED)
	return ::parse_object(cmd, type, co, d);
    
    if (!(i = sizeof(cmd)))
	return 0;
    rpname = query_pname();
    rname = query_name();
    for (name=""; i--;)
    {
	name = cmd[i] + name;
	if (name == herb_name)
	{
	    cmd[i] = rname;
	    return ::parse_object(cmd[..i], type, co, d);
	}
	if (name == herb_pname)
	{
	    cmd[i] = rpname;
	    return ::parse_object(cmd[..i], type, co, d);
	}
	name = " " + name;
    }
    return ::parse_object(cmd, type, co, d);
}

string
long()
{
    if (query_analysed(this_player()) == ITEM_ANALYSED)
	return ::long() + "It's name is " + capitalize(herb_name) + ".\n";
    return ::long();
}
