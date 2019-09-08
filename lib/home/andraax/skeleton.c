inherit "/base/living/";

#include <combat.h>
#include <slots.h>
#include <skills.h>

static nomask void
init_skeleton()
{
    int vol;

    set_race("skeleton");
    vol = query_volume();
    add_tool_slot(SLT_HEAD+1,    "head",          vol *  5);
    add_tool_slot(SLT_NECK+1,    "neck",          vol *  4);
    add_tool_slot(SLT_CHEST+1,   "chest",         vol * 19);
    add_tool_slot(SLT_BACK+1,    "back",          vol * 19);
    add_tool_slot(SLT_ARM+1,     "right arm",     vol *  7);
    add_tool_slot(SLT_ARM+2,     "left arm",      vol *  7);
    add_tool_slot(SLT_HAND+1,    "right hand",    vol *  4);
    add_tool_slot(SLT_HAND+2,    "left hand",     vol *  4);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       vol * 15);
    add_tool_slot(SLT_LEG+1,     "right leg",     vol * 16);
    add_tool_slot(SLT_LEG+2,     "left leg",      vol * 16);
    add_tool_slot(SLT_FOOT+1,    "right foot",    vol *  3);
    add_tool_slot(SLT_FOOT+2,    "left foot",     vol *  3);

    set_connections(([
	SLT_LEG+1:({ SLT_FOOT+1 }),
	SLT_LEG+2:({ SLT_FOOT+2 }),
	SLT_ARM+1:({ SLT_HAND+1 }),
	SLT_ARM+2:({ SLT_HAND+2 }),
	]));
    cb_set_parry_skill(SK_ATTACK_BARE_FIST);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	  return ({ SK_ATTACK_BARE_FIST, ATMOD_BARE_FIST,
			OBMOD_BARE_FIST });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 5 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 19 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_SKIN, 7 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ A_SKIN, 4 });
      case (SLT_ABDOMEN + 1):
	return ({ A_SKIN, 15 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ A_SKIN, 16 });
    }
    return 0;
}
