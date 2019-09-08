
inherit "/base/npc/humanoid";

#include <event.h>
#include <combat.h>
#include <living.h>
#include <race.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

static void
create_living()
{
    set_name("myorc");
    set_race("orc");
    set_long("A dumb looking Orc.\n");
    if (random(2))
	set_gender(G_MALE);
    else
	set_gender(G_FEMALE);
	
    add_lwvs(0, 123000, 125000, DWARF_MIN + random(HUMAN_MAX-HUMAN_MIN));

    set_tmp_stat(STAT_CO, 70 + random(40));
    set_tmp_stat(STAT_AG, 30 + random(20));
    set_tmp_stat(STAT_SD, 30 + random(20));
    set_tmp_stat(STAT_ME, 20 + random(20));
    set_tmp_stat(STAT_RE, 40 + random(20));
    set_tmp_stat(STAT_ST, 60 + random(40));
    set_tmp_stat(STAT_QU, 50 + random(20));
    set_tmp_stat(STAT_PR, 05 + random(25));
    set_tmp_stat(STAT_EM, 15 + random(20));
    set_tmp_stat(STAT_IN, 10 + random(10));
    set_tmp_stat(STAT_AP, 00 + random(50));

    set_skill_rank(SK_ATTACK_DAGGER, 2 + random(5));
    set_skill_rank(SK_ATTACK_CLUB, 2 + random(5));
    set_skill_rank(SK_ATTACK_SHORTSWORD, 2 + random(5));
    set_skill_rank(SK_PLATE, 2 + random(15));
    set_skill_rank(SK_CHAIN, 2 + random(10));
    set_skill_rank(SK_RIGID_LEATHER, 2 + random(5));
    set_skill_rank(SK_TRADING, random(25));
    set_skill_rank(SK_BLACK_SPEECH, 20);
    set_skill_rank(SK_ATTACK_BARE_FIST, 15);
    
    set_base_hp(65 + random(10) * 8);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 1500);
    set_fatigue(query_max_fatigue());

    add_prop(LIVE_USED_LANGUAGE, SK_BLACK_SPEECH);
    add_prop(LIVE_FIGHT_MOOD, 75);

    init_humanoid();
    if (random(2))
	wield_tool(add_equipment("/lib/weapon/one_h_s/dagger"));
    else
	wield_tool(add_equipment("/lib/weapon/one_h_c/club"));
}

