inherit "/base/npc/humanoid";
inherit "/base/brain/prefs";
inherit "/base/brain/small_fight";

#include <combat.h>
#include <event.h>
#include <living.h>
#include <race.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

void
create_living()
{
    int vol;

    set_name("armie");
    set_race("hobbit");
    set_long("He seems to be the mutant hobbit, living only for Aule.\n");
    set_gender(G_MALE);
    add_lwvs(0, 23000, 25000, HOBBIT_MIN + random(HOBBIT_MAX-HOBBIT_MIN));

    set_tmp_stat(STAT_CO, 50 + random(30));
    set_tmp_stat(STAT_AG, 50 + random(30));
    set_tmp_stat(STAT_SD, 30 + random(20));
    set_tmp_stat(STAT_ME, 40 + random(20));
    set_tmp_stat(STAT_RE, 40 + random(20));
    set_tmp_stat(STAT_ST, 40 + random(30));
    set_tmp_stat(STAT_QU, 50 + random(20));
    set_tmp_stat(STAT_PR, 25 + random(25));
    set_tmp_stat(STAT_EM, 35 + random(20));
    set_tmp_stat(STAT_IN, 40 + random(20));
    set_tmp_stat(STAT_AP, 40 + random(20));

    set_skill_rank(SK_ATTACK_DAGGER, 2 + random(5));
    set_skill_rank(SK_ATTACK_SHORTSWORD, 2 + random(5));
    set_skill_rank(SK_PLATE, 2 + random(15));
    set_skill_rank(SK_CHAIN, 2 + random(10));
    set_skill_rank(SK_RIGID_LEATHER, 2 + random(5));
    set_skill_rank(SK_TRADING, random(25));
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_SINDARIN, random(15));
    set_skill_rank(SK_ATTACK_BARE_FIST, 5);
    
    set_base_hp(4000 + random(10) * 8);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 500);
    set_fatigue(query_max_fatigue());
    
    vol = query_volume() / 100;

    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
    add_prop(LIVE_FIGHT_MOOD, 75);
		    
    init_humanoid();
    
    wear_tool(add_equipment("/lib/armor/s_leather/bracer"));
    wear_tool(add_equipment("/lib/armor/s_leather/bracer"));
    wear_tool(add_equipment("/lib/armor/plate/hobbit_plate"));
    wear_tool(add_equipment("/lib/armor/s_leather/small_helmet"));

    if (random(2))
	wield_tool(add_equipment("/lib/weapon/one_h_s/dagger"));
    else
	wield_tool(add_equipment("/lib/weapon/one_h_c/whip"));

    brain_init_prefs();
    add_event(E_ATTACKED_BY, "react_on_attack", this_object());
}
