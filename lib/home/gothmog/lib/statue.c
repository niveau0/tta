inherit "/base/object";


#include <material.h>
#include <event.h>
#include <properties.h>
#include <macros.h>

static void
create_object()
{
    set_name("statue");
    set_name("huge statue");
    set_short("statue");
    set_pshort("statues");
    set_long("This is the statue of a large winged demon.\n" +
             "It looks really lifelike, even the finest hair\n" +
             "seems to be minutely carved out.\n");
    add_adj("winged");
    add_lwvs( 20, 50    , 60    , 12 );
    add_prop(OBJ_MATERIAL, M_STONE);
    add_event(E_MOVE, "Move", this_object());
}

nomask int *
query_range()
{
    return ({ 20, 20, 20 });
}

nomask int
Move(object ob, object from, object to)
{
    if (ob == this_object())
    {
	remove_event(E_BLOCK, "E_block", from);
	if (to->query_prop(IS_ROOM))
	{
	    add_event(E_BLOCK, "E_block", to);
	}
    }
}

nomask int
E_block(object ob)
{
    object orc;


    if (!ob || ob == this_object())
	return 0;
    if ((ob->query_name()) == "gothmog")
    {    ob->catch_tell("Hail, Gothmog!\n");
	 ob->catch_tell("thisob" + this_object()->query_name() + "\n");
         orc = clone_object("/home/gothmog/lib/myorc");
         orc->move(E(this_object()));
	 ob->catch_tell("orc " + orc->query_name() + "\n");
	 return 0;
    }                                                        
    
    
    orc = clone_object("/home/gothmog/lib/myorc");
    orc->move(E(this_object()));
}
