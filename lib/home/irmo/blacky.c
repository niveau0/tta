inherit "/base/living";
inherit "/base/brain/findway";

#include <macros.h>
#include <living.h>
#include <state.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <properties.h>

int next_act, next_walk;
mapping known;
object target;

void
create_living()
{
    int vol;

    set_race_name("dog");
    set_name("blacky");
    set_living_name("blacky");
    add_name("dog");
    set_long("A cute black dog.\n");
    set_gender(1);

    set_level(10);
    set_base_hp(80);
    set_hp(query_max_hp());
    set_move_msg("trots");
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 500);

    add_lwvs(0, 20000, 20000, 100);

    vol = query_volume() / 100;

    add_tool_slot(SLT_HEAD+1,    "head",          55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "teeth",               1);
    add_tool_slot(SLT_CHEST+1,   "chest",         55 * 21);
    add_tool_slot(SLT_ARM+1,     "right foreleg", 55 *  7);
    add_tool_slot(SLT_ARM+2,     "left foreleg",  55 *  7);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       55 * 16);
    add_tool_slot(SLT_LEG+1,     "right hindleg", 55 * 16);
    add_tool_slot(SLT_LEG+2,     "left hindleg",  55 * 16);
    
    set_skill_rank(SK_WESTRON, 3);
    set_tell_flags(MSG_SEE | MSG_HEAR);
    
    init_brain();
    known=([]);
    target=0;
    
    next_act=call_out("blacky_act", 2);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	  return ({ SK_ATTACK_BARE_FIST, 114, 159, 250, 290, 315,
		    ({ CRUSH }) });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ ATT_SKIN, 0, 70, 6 });
      case (SLT_CHEST + 1):
	return ({ ATT_SKIN, 0, 70, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ ATT_SKIN, 0, 70, 7 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ ATT_SKIN, 0, 70, 3 });
      case (SLT_ABDOMEN + 1):
	return ({ ATT_SKIN, 0, 70, 16 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ ATT_SKIN, 0, 70, 16 });
    }
    return 0;
}


/*
 * Function name: blacky_act
 * Description:   do something
 */
static void
blacky_act()
{
    int *co, sz;

    if (!living(this_object()))
	return;

    next_act=call_out("blacky_act", 20+random(20));
    

    switch (random(20))
    {
      case 0:
	  command("eyebrow");
	return;
      case 1:
	  command("frown");
	return;
      case 2:
	  command("ponder");
	return;
      case 3:
	  command("exa irmo");
	return;
      case 4:
	  if (sz = sizeof(known))
	      command("exa " +
		  (map_indices(known))[random(sz)]);
      case 5:
      case 6:
	  command("stand up");
	return;
      case 7:
	  command("sit");
	return;
      case 8:
	  command("grin");
	return;
      case 9:
      case 10:
	  command("yawn");
	return;
      case 11:
	  command("sit");
	return;
    }
}

static void
blacky_walk()
{
    int result;
    
    if (!target
	|| (result = brain_approach(target)) < 0
	|| result == 3)
    {
	target=0;
	next_act=call_out("blacky_act", 20+random(20));
	return;
    }

    next_walk=call_out("blacky_walk", 1);
}

int
catch_tell(string msg, int no_prompt)
{
    string who, how;
    object pl, me;
    object *cmd;


    if (sscanf(msg, "%s cuddles you", who))
    {
	if (pl=present(who=lower_case(who), environment(me=this_object())))
	{
	    known[pl]=1;
	    command("lick "+who);
	}
	return 1;
    }

    if (sscanf(msg, "%s pats you %s", who, how))
    {
	if (pl=present(who=lower_case(who), environment(me=this_object())))
	    switch (how)
	    {
	      case "on your back":
		  command("sit");
		  break;
	    }
    }

    if (sscanf(msg, "%s whistles %s at you.", who, how))
    {
	if (pl=present(who=lower_case(who), environment(me=this_object())))
	    switch (how)
	    {
		case "aggressively":
		    known[pl]=0;
		    command("eyebrow " + who + " annoyedly");
		    break;
		case "angrily":
		    if (random(2))
		    {
			brain_approach(pl);
			target=0;
			command("eyebrow " + who + " questioningly");
		    }
		    else
			command("emotehear barks angrily.");
		    break;
		case "fiercely":
		    brain_approach(pl);
		    if (random(5))
		    {
			target=pl;
			next_walk=call_out("blacky_walk", 1);
			remove_call_out(next_act);
		    }
		    else
		    {
			target=0;
			command("eyebrow " + who + " questioningly");
		    }
		    break;
	      case "lovingly":
	      case "sweetly":
		  switch (random(3))
		  {
		    case 0:
			brain_approach(pl);
			target=0;
			command("eyebrow " + who + " questioningly");
			break;
		    case 1:
			brain_approach(pl);
			target=pl;
			next_walk=call_out("blacky_walk", 1);
			remove_call_out(next_act);
			break;
		    case 2:
			command("eyebrow " + who + " lazily");
			break;
		  }
		  break;
	      default:
		  command("eyebrow " + who + " charmingly");
	    }
	return 1;
    }

    return 0;
}
