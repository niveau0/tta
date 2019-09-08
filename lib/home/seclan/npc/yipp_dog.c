inherit "/base/living";

#include <combat.h>
#include <living.h>
#include <macros.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

static void
create_living()
{
    int i;

    set_name("dog");
    add_name("yipper");
    set_race("dog");
    set_long_bs("Four legs, long filthy hair and several sharp yellow teeth "+
	     "among this conglomeration are the main components of this "+
	     "dog .\n");
    add_adj("annoying");
    set_gender(G_MALE);
    add_lwvs(0, 10000, 10000, 16);

    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 15 ); /* Werte ueberdenken */
	set_pot_stat(i, 20 );
    }
    set_tmp_stat(STAT_AP, 3 );

    set_skill_rank(SK_ATTACK_TINY_ANIMAL, 20);
    set_move_msg("tripples");
    
    set_base_hp(3);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 10);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_HEAD+1,    "head",          0);
    add_tool_slot(SLT_EAR+1,     "right ear",     0);
    add_tool_slot(SLT_EAR+2,     "left ear",      0);
    add_tool_slot(SLT_EYE+1,     "right eye",     0);
    add_tool_slot(SLT_EYE+2,     "left eye",      0);
    add_tool_slot(SLT_NOSE+1,    "nose",          1);
    add_tool_slot(SLT_TONGUE+1,  "tongue",        10);
    add_tool_slot(SLT_NECK+1,    "neck",          100);
    add_tool_slot(SLT_MOUTH+1,   "mouth",         400);
    add_tool_slot(SLT_CHEST+1,   "chest",         0);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       0);
    add_tool_slot(SLT_LEG+1,     "right foreleg", 0);
    add_tool_slot(SLT_LEG+2,     "left foreleg",  0);
    add_tool_slot(SLT_LEG+3,     "right backleg", 0);
    add_tool_slot(SLT_LEG+4,     "left backleg",  0);
    add_tool_slot(SLT_FOOT+1,    "right forepaw", 2);
    add_tool_slot(SLT_FOOT+2,    "left forepaw",  2);
    add_tool_slot(SLT_FOOT+3,    "right backpaw", 2);
    add_tool_slot(SLT_FOOT+4,    "left backpaw",  2);

    set_connections(([
	SLT_HEAD+1:({ SLT_EYE+1, SLT_EYE+2, SLT_EAR+1, SLT_EAR+2,
			  SLT_MOUTH+1, SLT_NECK+1 }),
	SLT_MOUTH+1:({ SLT_TONGUE+1, SLT_NOSE+1 }),
	SLT_NECK+1:({ SLT_CHEST+1 }),
	SLT_CHEST+1:({ SLT_LEG+1, SLT_LEG+2, SLT_ABDOMEN+1 }),
	SLT_ABDOMEN+1:({ SLT_LEG+3, SLT_LEG+4 }),
	SLT_LEG+1:({ SLT_FOOT+1 }),
	SLT_LEG+2:({ SLT_FOOT+2 }),
	SLT_LEG+3:({ SLT_FOOT+3 }),
	SLT_LEG+4:({ SLT_FOOT+4 }),
	]));
    if (IS_CLONE) // prevents the master_object from causing call_outs
	call_out("yipp", 1, 0 );
}

void
yipp(int yipp_number)
{
    if (yipp_number+random(15) > 18)
    {
	command("emotesee stiffens abruptly.");
	command("emotesee scratches his chest with fast movements.");
	command("emotesee rests exhaustedly.");
	call_out("yipp", 7, 0 );
    }
    else
    {
	if (!yipp_number)
	    command("emotesee starts staring at you with frenzy eyes.");
	command("emotehear yipps wildly.");
	call_out("yipp", 3, ++yipp_number );
    }
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_MOUTH + 1):
	  return ({ SK_ATTACK_BITE,
			ATMOD_BITE,
			OBMOD_BITE });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 25 });
      case (SLT_EYE + 1):
      case (SLT_EYE + 2):
	return ({ A_SKIN, 3 });
      case (SLT_EAR + 1):
      case (SLT_EAR + 2):
	return ({ A_SKIN, 6 });
      case (SLT_MOUTH + 1):
	return ({ A_SKIN, 10 });
      case (SLT_NOSE + 1):
	return ({ A_SKIN, 5 });
      case (SLT_TONGUE + 1):
	return ({ A_SKIN, 4 });
      case (SLT_NECK + 1):
	return ({ A_SKIN, 12 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 30 });
      case (SLT_ABDOMEN + 1):
	return ({ A_SKIN, 30 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
      case (SLT_LEG + 3):
      case (SLT_LEG + 4):
	return ({ A_SKIN, 15 });
      case (SLT_FOOT + 1):
      case (SLT_FOOT + 2):
      case (SLT_FOOT + 3):
      case (SLT_FOOT + 4):
	return ({ A_SKIN, 8 });
    }
    return 0;
}
