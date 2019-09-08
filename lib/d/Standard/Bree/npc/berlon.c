inherit "/base/npc/shopkeeper";

#include <combat.h>
#include <living.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

# define DOMAINPATH "/d/Standard/Bree/"

static void
create_living()
{
    add_lwvs(0, 95000, 90000, 65); // This has to happen before init_shopkeeper
    init_shopkeeper();
    set_name("berlon");
    add_name("barkeeper");
    add_name("landlord");
    set_race("human");
    set_long("You see a man of less than mediocre height. Black hair "+
	     "covers his head and also build two thick brows over his "+
	     "eyes. Though he is shaved you notice dark shadows on his "+
	     "chin. Considering his build he is strong and muscular, "+
	     "but he has a tendency to be fat.\n");
    add_adj("fat");
    add_adj("little");
    set_gender(G_MALE);

    set_skill_rank(SK_BODY_DEVELOP, 4);
    set_skill_rank(SK_PERCEPTION, 10);
    set_skill_rank(SK_TRADING, 16);
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_ATTACK_BARE_FIST, 12); /* Werte ueberdenken */
    set_skill_rank(SK_ATTACK_BASH, 1);
    
    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
    add_prop(LIVE_NPC_MAX_FATIGUE, 50);

    set_tmp_stat(STAT_CO, 50 );
    set_pot_stat(STAT_CO, 50 );
    set_tmp_stat(STAT_AG, 50 );
    set_pot_stat(STAT_AG, 50 );
    set_tmp_stat(STAT_SD, 35 );
    set_pot_stat(STAT_SD, 35 );
    set_tmp_stat(STAT_ME, 60 );
    set_pot_stat(STAT_ME, 60 );
    set_tmp_stat(STAT_RE, 40 );
    set_pot_stat(STAT_RE, 40 );
    set_tmp_stat(STAT_ST, 60 );
    set_pot_stat(STAT_ST, 60 );
    set_tmp_stat(STAT_QU, 35 );
    set_pot_stat(STAT_QU, 35 );
    set_tmp_stat(STAT_PR, 15 );
    set_pot_stat(STAT_PR, 15 );
    set_tmp_stat(STAT_EM, 10 );
    set_pot_stat(STAT_EM, 10 );
    set_tmp_stat(STAT_IN, 20 );
    set_pot_stat(STAT_IN, 20 );
    set_tmp_stat(STAT_AP, 33 );
    set_pot_stat(STAT_AP, 33 );

    set_base_hp(37);
    set_hp(query_max_hp());
    set_fatigue(query_max_fatigue());
    
    add_equipment(DOMAINPATH+"room/pub/obj/pub_key");
    set_shop(DOMAINPATH+"room/pub/pub");
}
