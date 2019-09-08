/* A shopkeeper */

inherit "/base/npc/shopkeeper";

#include <living.h>
#include <properties.h>
#include <skills.h>

static void
create_living()
{
    set_race("hobbit");
    set_name("fritan");
    add_adj("small");
    set_long("He is a small and busy shopkeeper.\n");
    set_gender(G_MALE);
    set_base_hp(35);
    set_hp(query_max_hp());

    set_skill_rank(SK_BODY_DEVELOP, 4);
    set_skill_rank(SK_PERCEPTION, 10);
    set_skill_rank(SK_TRADING, 16);
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_ATTACK_DAGGER, 10 + random(10));
    
    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
    add_prop(LIVE_NPC_MAX_FATIGUE, 50);
    add_lwvs(0, 23000, 25000, 24);
    init_shopkeeper();
    add_equipment("/d/Standard/Bree/obj/tailor_key");
    set_shop("/d/Standard/Bree/room/tailor");
}
