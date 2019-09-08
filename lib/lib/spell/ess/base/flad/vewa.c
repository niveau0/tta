inherit "../../ess_base";

#include <magic.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>


string
query_desc()
{
    return "Break a random limb of the target.";
}

void
spell_effect(int level, object *tar, object tp)
{
    int *slots, slot;
    string slot_name;
    
    if (!sizeof(tar))
	return;

    if (tar[0] == tp)
    {
	tp->catch_tell("Want to kill yourself?\n");
	return;
    }

    slots = tar[0]->query_slots();
    slot = slots[random(sizeof(slots))];
    slot_name = tar[0]->query_slot_desc(slot);
    if (tar[0]->query_wounds()[slot] > SLOT_UNUSABLE) 
    {
	tp->catch_tell("Your spell has no effect on your foe's " + slot_name +
		       ".\n");
	return;
    }
    tar[0]->set_wound(slot, SLOT_BROKEN);
    tar[0]->catch_tell("Suddenly your " + slot_name + " breaks without " +
		       "been hit.\n");
    tp->catch_tell("Your spell breaks your foe's " + slot_name + ".\n");
}

int
query_level()
{
    return 5;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    10000,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
