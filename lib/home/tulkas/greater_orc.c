inherit "/base/npc/humanoid";
 
#include <combat.h>
#include <living.h>
#include <race.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>


static nomask void equip();

static void
create_living()
{
    set_name("uruk hai");
    set_race("orc");
    set_long("A big, nasty orc.\n");
    set_gender(G_MALE);
    add_lwvs(0, 73000, 55000, HUMAN_MIN + random(HUMAN_MAX - HUMAN_MIN));

    set_tmp_stat(STAT_CO, 90 + random(30));
    set_tmp_stat(STAT_AG, 60 + random(30));
    set_tmp_stat(STAT_SD, 90 + random(20));
    set_tmp_stat(STAT_ME, 50 + random(20));
    set_tmp_stat(STAT_RE, 50 + random(20));
    set_tmp_stat(STAT_ST, 90 + random(30));
    set_tmp_stat(STAT_QU, 50 + random(20));
    set_tmp_stat(STAT_PR, 35 + random(25));
    set_tmp_stat(STAT_EM, 45 + random(20));
    set_tmp_stat(STAT_IN, 40 + random(20));
    set_tmp_stat(STAT_AP, 40 + random(20));
    
    set_base_hp(80 + random(10) * 8);
    set_hp(query_max_hp());

    init_humanoid();
    
    set_skill_rank(SK_PLATE, 5 + random(15));
    set_skill_rank(SK_CHAIN, 5 + random(10));
    set_skill_rank(SK_RIGID_LEATHER, 5 + random(5));
    set_skill_rank(SK_BLACK_SPEECH, 30);
    set_skill_rank(SK_ATTACK_BARE_FIST, 5);

    add_prop(LIVE_NPC_MAX_FATIGUE, 1000);

    set_fatigue(query_max_fatigue());

    add_prop(LIVE_USED_LANGUAGE, SK_BLACK_SPEECH);
    add_prop(LIVE_FIGHT_MOOD, 75);

    wear_tool(add_equipment("/lib/armor/s_leather/hobbit_bracer"));
    wear_tool(add_equipment("/lib/armor/r_leather/hobbit_bracer"));
    wear_tool(add_equipment("/lib/armor/plate/hobbit_plate"));
    wear_tool(add_equipment("/lib/armor/s_leather/small_helmet"));

    equip();
}


static nomask void
equip()
{
    int i;

    i = random(2);

    if (i = 0)
    {        
        set_skill_rank(SK_ATTACK_HAND_AXE, 5 + random(5));

        wield_tool(add_equipment("/lib/weapon/one_h_s/handaxe"));
    }   

    if (i = 1)
    {
        set_skill_rank(SK_ATTACK_LONGSWORD, 5 + random(5));

        wield_tool(add_equipment("/lib/weapon/one_h_s/longsword"));
    }
}
