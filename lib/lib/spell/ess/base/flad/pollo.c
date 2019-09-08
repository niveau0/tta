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
    int *slots, slot, failure;
    string slot_name;
    
    if (!sizeof(tar))
	return;

    if (tar[0] == tp)
    {
	tp->catch_tell("Want to kill yourself?\n");
	return;
    }


    failure = tar[0]->query_prop(LIVE_LAST_RR_RESULT) * (-1);
    slots = tar[0]->query_slots();
    slot = slots[random(sizeof(slots))];
    slot_name = tar[0]->query_slot_desc(slot);
    debug("aule", failure);
    debug("aule", slot);
    if (tar[0]->query_wounds()[slot] == SLOT_LOST) 
    {
	tp->catch_tell("Your spell has no effect on your foe's " + slot_name +
		       ".\n");
	return;
    }    
    tar[0]->catch_tell("Suddenly an immense pain hits your " +
		       slot_name + ".\n");
    tp->catch_tell("Your spell hits your foe's " + slot_name + ".\n");
    switch (failure)
    {
      case 0..10:
	  CRITICAL(IMPACT, ({ slot }), tar[0], tp, 1, 1);
	  break;
      case 11..20:
	  CRITICAL(IMPACT, ({ slot }), tar[0], tp, 7, 2);
	  break;
      case 21..30:
	  CRITICAL(IMPACT, ({ slot }), tar[0], tp, 12, 2);
	  break;
      default:
	  CRITICAL(IMPACT, ({ slot }), tar[0], tp, 17, 2);
	  break;
    }
}

int
query_level()
{
    return 3;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_NONE,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
