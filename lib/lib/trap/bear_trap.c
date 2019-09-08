inherit "/base/object";

#include <combat.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>
#include <skills.h>

static object trapped, dropped_by;

static void
create_object()
{
    set_name("trap");
    set_name("bear trap");
    set_short("bear trap");
    set_pshort("bear traps");
    set_long("It's a bear trap with sharp jags.\n");
    
    add_prop(OBJ_MATERIAL, M_IRON);
    add_lwvs(0, 4000, 7500, 20);
    set_value(500);
    add_event(E_MOVE, "E_move", this_object());
}

nomask int *
query_range()
{
    return ({ 20, 20, 20 });
}

nomask int
E_move(object ob, object from, object to)
{
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
    if (!ob || ob == this_object())
	return 0;
    
    if (trapped == ob)
    {
	ob->catch_tell("Ouch! You are trapped.\n");
	ob->set_coordinates(query_coordinates());
	ob->check_death(random(3), this_object());
	return 1;
    }
    if (trapped)
	return 0;
    
    if (ob == dropped_by)
    {
	ob->catch_tell("You avoid stepping into your own trap.\n");
	return 0;
    }
    if (VISIBLE(ob, this_object()))
    {
	ob->catch_tell("You avoid stepping into the trap.\n");
	return 0;
    }
    trapped = ob;
    add_prop(OBJ_HIDE, 0);
    add_prop(OBJ_HIDE_PLACES, 0);
    ob->catch_msg(({ "Argh! You stepped into ", QNAME(this_object()), ".\n" }),
		  this_object(), MSG_SEE);
    tell_objects(LISTEN(E(this_object())) - ({ ob }),
		 ({ QCTNAME(ob), " screams, " + ob->query_pronoun() +
			" stepped into ", QNAME(this_object()), ".\n" }),
		 MSG_HEAR, ob, 600);
    ob->set_coordinates(query_coordinates());
    ob->check_death(random(10) + 5, this_object());
    return 1;
}

nomask int
disarm_cmd(object ob)
{
    int mod, result;

    mod = 0;
    
    if (!ob || ob == this_object())
	return 0;

    if (trapped == ob)
	mod -= 20;

    if (ob == dropped_by)
	mod += 20;

    result = ob->skill_check(SK_DISARM_TRAP, mod, 0);
    debug("aule",result);
    if (result >= 100)
    {
	if (ob == trapped)
	{
	    ob->echo("You manage to free yourself from this nasty " +
			   "trap.\n");
	    trapped = 0;
	    return 1;
	}
	if (ob == dropped_by)
	    ob->echo("You manage to disarm your trap.\n");
	else
	    ob->echo("You manage to disarm the trap.\n");
	if (trapped && living(trapped))
	    trapped->catch_tell("You are very pleased when the trap is " +
				"opened.\n");
	trapped = 0;
	tell_objects(LISTEN(E(this_object())) - ({ ob }),
		     ({ QCTNAME(ob), " disarms a bear trap.\n" }),
		     MSG_HEAR, ob, 600);
	return 1;
    }
    else if (result < 20)
    {
	if (!trapped)
	{
	    trapped = ob;
	    ob->catch_msg(({ "Argh! You stepped into ", QTNAME(this_object()),
				 ".\n" }),
			  this_object(), MSG_SEE);
	    tell_objects(LISTEN(E(this_object())) - ({ ob }),
			 ({ QCTNAME(ob), " screams, when " +
				ob->query_pronoun() +
				" stepped into ", QTNAME(this_object()),
				".\n" }),
			 MSG_HEAR, ob, 600);
	    ob->set_coordinates(query_coordinates());
	    ob->check_death(random(10) + 5, this_object());
	    return 1;
	}
	else if (trapped != ob)
	{
	    trapped->catch_tell("Ouch! When ob bla bla bla tried to disarm " +
				"the trap " +ob->query_pronoun()+
				" obviously made a mistake.\n");
	    trapped->check_death(random(3), this_object());
	}
	if (random(2))
	{
	    ob->catch_tell("Damned. You cut yourself, when you tried to "+
			   "disarm it.\n");
	    ob->check_death(random(3) + 3, this_object());
	}
	else
	    ob->catch_tell("Phew, that was close. The trap opened a bit and "+
			   "closed that fast again that you almost cut " +
			   "yourself.\n");
    }
    else
    {
	ob->catch_tell("You are not able to disarm this trap.\n");
	if (trapped && !(ob == trapped))
	    trapped->catch_tell("Ob bla bla doesn't seem to be able to " +
				"disarm the trap.\n");
	return 1;
    }
    tell_objects(LISTEN(E(this_object())) - ({ ob }),
		 ({ QCTNAME(ob), " tried to disarm a bear trap, but fails. "+
			"\n" }),
		 MSG_HEAR, ob, 600);
}
