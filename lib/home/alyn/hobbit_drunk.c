inherit "/base/npc/humanoid";
inherit "/system/money";

#include <combat.h>
#include <macros.h>
#include <living.h>
#include <race.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>
#include <money.h>

static void
create_living()
{
    object purse, belt;
    
    set_name("hobbit");
    set_race("hobbit");
    set_long_bs("This male hobbit has a big red nose and red glowing " +
	"cheeks. His hair is greasy and filthy.\n");
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
    set_tmp_stat(STAT_AP, 20 + random(20));

    set_skill_rank(SK_ATTACK_DAGGER, 2 + random(5));
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_SINDARIN, random(15));
    set_skill_rank(SK_ATTACK_BARE_FIST, 5);
    
    set_base_hp(40 + random(10) * 8);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 500);
    set_fatigue(query_max_fatigue());

    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
    add_prop(LIVE_FIGHT_MOOD, 10);

    init_humanoid();

    if (IS_CLONE)
    {
	add_equipment("/lib/drink/winebottle");
	belt=add_equipment("/lib/misc/equipment/small_belt");
	wear_tool(add_equipment("/lib/armor/cloth/hobbit_underwear"));
	wear_tool(add_equipment("/home/alyn/hobbit_linen_shirt"));
	wear_tool(add_equipment("/home/alyn/hobbit_cotton_vest"));
	wear_tool(add_equipment("/home/alyn/hobbit_linen_pants"));
	purse=add_equipment("/lib/misc/container/purse");
	make_coins(2, SILVER_ROYAL, purse);
	make_coins(20, BRONZE_COPPER, purse);
	purse->add_prop(CONT_CLOSED, 1);
	wear_tool(belt);
	command("sit down");
	call_out("behaviour",0,this_object());
    }
}
static void
behaviour(object drunk)
{
    switch(random(4))
    {
      case (0):
	  command("hiccup");
	  break;
      case (1):
	  command("burp");
	  break;
      case (2):
	  command("yawn");
	  break;
      case (3):
	  command("drink from bottle");
	  break;
      default:
      {
      }
    }
    call_out("behaviour",40 + random(10),drunk);
}
