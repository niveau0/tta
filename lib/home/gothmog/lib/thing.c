inherit "/base/weapon";

#include <properties.h>
#include <event.h>
#include <macros.h>

static object trapped,dropped_by;

static void
create_weapon()
{
    set_name("thing");
    set_short("thing");
    set_pshort("things");
    set_long("What shall I say ? It`s a thing.\n");
    set_value(0);
    add_lwvs(100,1,1,1);
    add_prop(IS_WIELDABLE, 1);
    set_wield_size(100, -2);
    add_event(E_WIELD, "E_wield", this_object());
    add_event(E_REMOVE_TOOL, "E_unwield", this_object()); 
    add_event(E_MOVE, "E_mymove", this_object());
}    


nomask int *
query_range()
{
    return ({ 20, 20, 20 });
}

nomask int
E_mymove(object ob, object from, object to)
{
    this_player()->catch_tell(" ob ist : " + (ob->query_name()) + "\n");
    this_player()->catch_tell(" from ist : " + (from->query_name()) + "\n");
    this_player()->catch_tell(" to ist : " + (to->query_name()) + "\n");
    if (ob == this_object())
    {
	remove_event(E_BLOCK, "E_block", from);
	if (to->query_prop(IS_ROOM))
	{
	    add_event(E_BLOCK, "E_block", to);
	    dropped_by = this_player();
	}
    }
}

nomask int
E_block(object ob)
{
    this_player()->catch_tell(" ob ist : " + (ob->query_name()) + "\n");
    if (!ob || ob == this_object())
	return 0;
    dropped_by->catch_tell("Test.\n");
}
    
static int
E_wield(object tp, object ob)
{
    tp->catch_tell("You feel a strange feeling as you start " +
                   "to wield the " + short() + "\n");
}    

static int
E_unwield(object tp, object ob)
{   object env;

    env=environment();
    if ((env->query_name()) != 0)
    {
	tp->catch_tell("You are overcome by sadness as you remove this " +
                    "wonderfull " + short() + "\n");
    }
}

