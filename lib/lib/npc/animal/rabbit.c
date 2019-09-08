inherit "/base/living";
inherit "/base/brain/small";

#include <combat.h>
#include <event.h>
#include <living.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_living()
{
    int i;
    
    set_name("rabbit");
    set_race("rabbit");
    set_long("A cute brown rabbit.\n");
    add_adj("brown");
    set_gender(G_NEUTER);
    add_lwvs(0, 6000, 5500, 20);

    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 10 + random(10));
	set_pot_stat(i, 10 + random(10));
    }
    set_tmp_stat(STAT_AP, 50 + random(30));
    set_tmp_stat(STAT_QU, 50 + random(30));

    set_skill_rank(SK_ATTACK_TINY_ANIMAL, 5);
    cb_set_parry_skill(SK_ATTACK_TINY_ANIMAL);
    set_move_msg("hops");
    set_run_msg("speeds");
    
    set_base_hp(7);
    set_hp(query_max_hp());

    add_prop(LIVE_NPC_MAX_FATIGUE, 50);
    set_fatigue(query_max_fatigue());

    add_tool_slot(SLT_HEAD+1,    "head",          55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "teeth",               1);
    add_tool_slot(SLT_CHEST+1,   "chest",         55 * 21);
    add_tool_slot(SLT_ARM+1,     "right foreleg", 55 *  7);
    add_tool_slot(SLT_ARM+2,     "left foreleg",  55 *  7);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       55 * 16);
    add_tool_slot(SLT_LEG+1,     "right hindleg", 55 * 16);
    add_tool_slot(SLT_LEG+2,     "left hindleg",  55 * 16);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_MOUTH + 1):
	  return ({ SK_ATTACK_TINY_ANIMAL, ATMOD_TINY_ANIMAL,
			OBMOD_TINY_ANIMAL });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 6 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_SKIN, 7 });
      case (SLT_ABDOMEN + 1):
	return ({ A_SKIN, 16 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ A_SKIN, 7 });
    }
    return 0;
}

void
E_move_inv(object ob, object from, object to)
{
    brain_flee(ob);
}
