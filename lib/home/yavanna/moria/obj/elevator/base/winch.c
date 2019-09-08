inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <event.h>
#include "../../../include/moria.h"

void
create_winch()
{}

void
create_object()
{
    set_name("winch");
    add_adj("wooden");
    set_short("wooden winch");
    set_pshort("wooden winches");
    set_long("A winch is here, fixed to the wall.\n"+
	     "Maybe you can turn it.\n");
    add_prop(OBJ_MATERIAL,M_OAK);
    add_lwvs(0,100,100,50);
    add_prop(OBJ_NO_GET,1);
    if(IS_CLONE)
	add_event(E_MOVE, "enter_env", this_object());
    create_winch();
}

nomask void
enter_env(object ob, object from, object to)
{
    if (!to || ob != this_object() || living(to))
       return;
   
    if(E(this_object())->query_prop(ELEVATOR_IS))
    {
	E(this_object())->set_winch(MASTER_OB(this_object()));
	remove_event(E_MOVE, "enter_env", this_object());
    }
}


int
turn_cmd(string dir)
{
    object tp;
    tp = this_player();

/*
  if(living(E(this_object())))
    {
	tp->catch_tell("You cannot turn something you are carrying!\n");
	return 1;
    }
    */  
    if (!dir)
    {
	tp->catch_tell("Turn " + query_name() +
		       " in which direction?\n");
	return 1;
    }
    tp->catch_tell("You turn the " + query_name() + " " + dir + ".\n");
    tell_objects(LISTEN(E(this_object())) - ({ tp }),
		({ QCTNAME(this_player()),
		       " turns ", QTNAME(this_object()),
		       " ", dir, ".\n" }), MSG_SEE, tp);


    if(dir == "right")
    {
	E(this_object())->start_move(DIRECTION_UP);
	return 1;
    }
    if(dir == "left")
    {
	E(this_object())->start_move(DIRECTION_DOWN);
	return 1;
    }
    tp->catch_tell("You cannot turn this.\n");
    return 1;
}
