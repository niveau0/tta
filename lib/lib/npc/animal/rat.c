inherit "/base/living";

#include <living.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <properties.h>

static void
create_living()
{
    int i;
    
    set_name("rat");
    set_race("rat");
    set_long("An ugly grey rat.\n");
    set_gender(G_NEUTER);
    add_adj("ugly");
    add_adj("grey");
    add_lwvs(0, 6000, 5500, 20);
    
    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 10 + random(10));
	set_pot_stat(i, 10 + random(10));
    }
    set_tmp_stat(STAT_AP, 10);
    set_tmp_stat(STAT_QU, 30 + random(30));

    set_skill_rank(SK_ATTACK_TINY_ANIMAL, 5);
    cb_set_parry_skill(SK_ATTACK_TINY_ANIMAL);
    set_move_msg("trips");
    set_run_msg("chases");
    
    set_base_hp(4);
    set_hp(query_max_hp());

    add_prop(LIVE_NPC_MAX_FATIGUE, 50);
    set_fatigue(query_max_fatigue());

    add_tool_slot(SLT_HEAD+1,    "head",         55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "fangs",              1);
    add_tool_slot(SLT_CHEST+1,   "body",         55 * 21);
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
    }
    return 0;
}
