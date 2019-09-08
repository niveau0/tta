inherit "/base/living";

#include <combat.h>
#include <living.h>
#include <macros.h>
#include <skills.h>
#include <slots.h>
#include <stdproperties.h>

/* Generelle Wertueberpruefung noetig. */

static void
create_living()
{
    int i;

    set_name("chicken");
    add_name("chick");
    set_race("chicken");
    set_long("A cute little chick that looks like a yellow featherball.\n");
    add_adj("yellow");
    add_adj("little");
    set_gender(G_NEUTER);
    add_lwvs(0, 35, 250, 5);

    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 2 ); /* Werte ueberdenken */
	set_pot_stat(i, 2 );
    }
    set_tmp_stat(STAT_AP, 60 + random(30));

    set_skill_rank(SK_ATTACK_TINY_ANIMAL, 1); /* 1 zu klein? */
    cb_set_unarmed_skill(SK_ATTACK_TINY_ANIMAL);
    set_move_msg("hops");
    set_run_msg("races");
    
    set_base_hp(1+random(1));
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 15);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_HEAD+1,    "head",           5);
    add_tool_slot(SLT_MOUTH+1,   "bill",           1);
    add_tool_slot(SLT_CHEST+1,   "chest",          9);
    add_tool_slot(SLT_ARM+1,     "right wing",     4);
    add_tool_slot(SLT_ARM+2,     "left wing",      4);
    add_tool_slot(SLT_LEG+1,     "right leg",      6);
    add_tool_slot(SLT_LEG+2,     "left leg",       6);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_MOUTH + 1):
	return ({ SK_ATTACK_TINY_ANIMAL, 114, 159, 250, 290, 315,
		      ({ PUNCTURE }) });
	/* WIE MUESSEN HIER WERTE AUSSEHEN !?!
	 * Im Moment noch Huehnerwerte !
	 */
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ ATT_SKIN, 0, 5, 7 });
      case (SLT_CHEST + 1):
	return ({ ATT_SKIN, 0, 5, 55 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ ATT_SKIN, 0, 5, 6 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ ATT_SKIN, 0, 5, 13 });
    }
    return 0;
}
