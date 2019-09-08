inherit "../../ess_base";

#include <combat.h>
#include <magic.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

string
query_desc()
{
    return "Hurt a random limb of the target critically.";
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
    if (tar[0]->query_wounds()[slot] == SLOT_LOST) 
    {
	tp->catch_tell("Your spell has no effect on your foe's " + slot_name +
		       ".\n");
	return;
    }    
    tar[0]->catch_tell("Suddenly an immense pain hits your " +
		       slot_name + ".\n");
    tp->catch_tell("Your spell hits your foe's " + slot_name + ".\n");
    CRITICAL(IMPACT, ({ slot }), tar[0], tp, 17, 2);
}

int
query_level()
{
    return 50;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_NONE,
	    30000,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
