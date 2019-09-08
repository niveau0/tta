inherit("/base/object");

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <skills.h>
#include <event.h>


int *status, *range, disarm_difficulty, disarm_fumble_range;

object *traps;

int activate();
void set_range(int x, int y, int z);
int set_trap(object new_trap);
void set_disarm_difficulty(int difficulty);
void set_fumble_range(int range);

void
create_activator()
{
}

void
create_object()
{
    set_name("activator");
    set_name("plate");
    set_name("stone-plate");
    add_adj("stone");
    set_short("stone-plate");
    set_pshort("stone-plates");
    set_long("The stone-plate is sunk 1-2 inch into the ground like pressed"+
	     " down by a giant.\n");
    add_prop(OBJ_MATERIAL, M_STONE);
    add_prop(OBJ_NO_GET,1);
//    add_prop(OBJ_NO_REMOVE_MSG,1);
//    add_prop(OBJ_NO_SHOW,1);
    status = ({ 0, 0 });
    traps = ({ });
    
    add_lwvs(0,12000,5000,200);
    set_value(0);
    set_range(36,36,0);
    set_disarm_difficulty(90);
    set_fumble_range(5);
    add_event(E_MOVE, "enter_env", this_object());
    activate();
    
    create_activator();
}

nomask int
enter_env(object ob, object dest, object old)
{
    if (!dest || ob != this_object())
	return 0;
    add_event(E_BLOCK,"E_block", dest);
    return 1;
}

string
long()
{
    if (!(status[1]))
	return ::long();
    return "The ground is covered by " + plural_short() +".\n";
}


int*
query_range()
{
    return range;
}

void
set_range(int x, int y, int z)
{
    range = ({ x, y, z });
}

void
set_disarm_difficulty(int difficulty)
{
    disarm_difficulty = difficulty;
}

int
query_disarm_difficulty()
{
    return disarm_difficulty;
}

void
set_fumble_range(int range)
{
    disarm_fumble_range = range;
}

int
query_fumble_range()
{
    return disarm_fumble_range;
}


int
activate()
{
    if (!(status[1]))
    {
	status[1] = 1;
	status[0] = 1;
	if (E(this_object()))
	    tell_objects(LISTEN(E(this_object())),
			 "With a 'click' the stone-plate is moved back to the"+
			 " ground-level.\n", MSG_SEE | MSG_HEAR,
			 this_object());
	add_prop(OBJ_NO_SHOW,1);
	return 1;
    }
}

int
deactivate()
{
    if (status[0])
    {
	status[0] = 0;
	return 1;
    }
}

int
pressed_down()
{
    if (status[0])
    {
	status[1] = 0;
	status[0] = 0;
	add_prop(OBJ_NO_SHOW, 0);
	return 1;
    }
    return 0;
}

object*
query_traps()
{
    return traps;
}

int
set_trap(object new_trap)
{
    traps += ({ new_trap });
    return 1;
}

int
remove_trap(object trap)
{
    traps -= ({ trap });
    return 1;
}

int
disarm_cmd(object tp)
{
    int    skill, roll,i;

    if ((!status[0]) || (!status[1]))
    {
	tp->catch_tell("Hmm, seems like this "+short()+" isn't dangerous.\n");
	return -1;
    }
    skill = tp->query_skill_bonus(SK_DISARM_TRAP);

    roll = roll_dice(-1) + skill;
    if (tp->query_race() == "dwarf")
	roll += 20;
    if (roll < disarm_difficulty)
    {
	tp->catch_tell("You failed.\n");
	if (roll <= query_fumble_range())
	{
	    tp->catch_tell("Argh, you set the trap off.\n");
	    tell_objects(LISTEN(E(this_object()))- ({ tp }),
			 ({ "Hmm, looks like ", QTNAME(tp), " sets off ",
				"a trap.\n" }), MSG_SEE, tp);
	    pressed_down();
	    for (i=sizeof(traps); i--;)
		traps[i]->do_attack(tp);
	}
	return -1;
    }
    if (deactivate())
	tp->catch_tell("You disarmed the trap.\n");
    return 1;
}


nomask int
E_block(object ob)
{
    int i, skill;
    
    if (!ob || ob == this_object())
	return 0;
    
    if (status[0])
    {
	if(roll_dice(-1) <= ob->query_skill_bonus(SK_PERCEPTION))
	{
	    ob->catch_tell("Hmm, one of the " + plural_short()+
			   " looks suspicious.\n");
	    return 0;
	}
	if (living(ob))
	{
	    tell_objects(LISTEN(E(this_object())) - ({ ob }),
			 ({ "The ", short(), " on which ", QTNAME(ob),
				" stands, sinks with a 'click' some ",
				"inches into the ground.\n" }),
			 MSG_SEE, ob);
	    ob->catch_tell("'Click', the "+short()+" sinks some inches "+
			   "into the ground.\n");
	}
	else
	    tell_objects(LISTEN(E(this_object())),
			 ({ "With a 'click' one of the ",plural_short(),
				" sinks into the ground when ", QTNAME(ob),
				" touches it.\n"}), MSG_SEE | MSG_HEAR,
				this_object());
	pressed_down();
	for (i=sizeof(traps); i--;)
	    traps[i]->do_attack(ob);
	return 1;
    }
    return 0;
}



