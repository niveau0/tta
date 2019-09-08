inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>
#include <event.h>

int slot;

string
query_desc()
{
    return "Sprains an arm of the target. Power manipulates the " +
	"time the victim suffers from the pain.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    int *slots, rounds, time;
    string slot_name;
    object ob;
    
    if (!sizeof(tar))
	return;

    if (tar[0] == tp)
    {
	tp->catch_tell("Want to kill yourself?\n");
	return;
    }


    rounds = tar[0]->query_prop(LIVE_LAST_RR_RESULT)/(-2);
    
    slots = tar[0]->query_slot_type(SLT_ARM);
    slot = slots[random(sizeof(slots))];
    slot_name = tar[0]->query_slot_desc(slot);
    if (tar[0]->query_wounds()[slot]) 
    {
	tp->catch_tell("Your spell has no effect on your foe's " + slot_name +
		       ".\n");
	return;
    }
    time = time();
    
    ob = clone_object(MASTER);
    tar[0]->cb_add_boni( ({ CB_DB_BONUS, CB_OB_BONUS }),
			 ({ ([ob : -20 ]), ([ob : -20]) }) );
    tar[0]->catch_tell("Suddenly an immense pain hits your " +
		       slot_name + ".\n");
    tp->catch_tell("Your spell hits your foe's " + slot_name + ".\n");
    tar[0]->add_spell(ob);
    ob->store_spell(tp, level, ({ tar[0] }));
    ob->start_call_out("remove_spell", rounds);
}
void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
	tar[0]->catch_tell("You can move your " +
			   tar[0]->query_slot_desc(slot) + "again.\n");
    destruct_object();
}

int
query_level()
{
    return 1;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
