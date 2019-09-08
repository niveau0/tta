inherit "/base/mount";

#include <combat.h>
#include <living.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static nomask void
create_mount()
{
    int i, rnd;
    
    set_name("horse");
    set_race("horse");
    set_long("A friendly snorting brown horse.\n");
    set_gender(G_NEUTER);
    add_adj("brown");
    add_lwvs(0, 300000, 330000, 85);
    
    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, (rnd = 50 + random(30)));
	set_pot_stat(i, rnd + random(10));
    }
    
    set_skill_rank(SK_ATTACK_BASH, 5);
    cb_set_parry_skill(SK_ATTACK_BASH);
    set_move_msg("trots");
    set_run_msg("gallops");

    set_base_hp(20);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 1000);
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
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	  return ({ SK_ATTACK_BASH, ATMOD_BASH, OBMOD_BASH });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 8 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_SKIN, 12 });
      case (SLT_ABDOMEN + 1):
	return ({ A_SKIN, 10 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ A_SKIN, 7 });
    }
    return 0;
}
