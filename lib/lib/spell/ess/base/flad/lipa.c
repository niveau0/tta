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
    return "Paralyses a random limb of the target.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    int *slots, rounds;
    string slot_name;
    object ob;
    
    if (!sizeof(tar))
	return;

    if (tar[0] == tp)
    {
	tp->catch_tell("Want to kill yourself?\n");
	return;
    }


    rounds = tar[0]->query_prop(LIVE_LAST_RR_RESULT)/(-5);
    
    slots = tar[0]->query_slots();
    slot = slots[random(sizeof(slots))];
    slot_name = tar[0]->query_slot_desc(slot);
    if (tar[0]->query_wounds()[slot]) 
    {
	tp->catch_tell("Your spell has no effect on your foe's " + slot_name +
		       ".\n");
	return;
    }
    tar[0]->set_wound(slot, SLOT_UNUSABLE);
    ob = clone_object(MASTER);
    tar[0]->catch_tell("Suddenly an immense pain paralyses your " +
		       slot_name + ".\n");
    tp->catch_tell("Your spell paralyses your foe's " + slot_name + ".\n");
    tar[0]->add_spell(ob);
    ob->store_spell(tp, level, ({ tar[0] }));
    ob->start_call_out("remove_spell", rounds);
}
void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
    {
	if (tar[0]->query_wounds()[slot]
	    != SLOT_UNUSABLE)
	{
	    destruct_object();
	    return;
	}
	tar[0]->catch_tell("You can move your " +
			   tar[0]->query_slot_desc(slot) + "again.\n");
	tar[0]->set_wound(slot, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 2;
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
