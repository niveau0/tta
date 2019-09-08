inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <slots.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Repairs a minor fracture.";
}

mixed
query_similar_code()
{
    return "_requer";
}

void
spell_effect(int level, object *tar, object tp, string extra)
{
    if (sizeof(tar))
    {
	int     slot;
	object  ob;
	mapping fracs;

	if (!map_sizeof(fracs = tar[0]->query_wounds()) ||
	    fracs[slot = tar[0]->query_desc_slot(extra)] != SLOT_BROKEN)
	{
	    if (tar[0] == tp)
		tp->catch_tell("You have no such minor fracture.\n");
	    else
		tp->catch_tell(capitalize(tar[0]->query_pronoun()) +
			       " has no such minor fracture.\n");
	    return;
	}
	if (level > 10)
	{
	    tar[0]->set_wound(slot, 0);
	    tar[0]->catch_tell("Your " + extra + " is no longer broken.\n");
	    return;
	}
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("heal_limb",
			   (86400 - level * 3600) / TIME_SCALE, slot);
	tar[0]->add_spell(ob);
	tar[0]->catch_tell("You feel your " + extra + " is healing faster.\n");
    }
}

static void
heal_limb(int slot)
{
    object tar;
    
    if (sizeof(query_targets()) && (tar = query_targets()[0]) &&
	tar->query_wounds()[slot] == SLOT_BROKEN)
    {
	tar->set_wound(slot, 0);
	tar->catch_tell("You feel that your " + tar->query_slot_desc(slot) +
			" is no longer broken.\n");
    }
    destruct_object();
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
	    SP_HEALING,
	    SP_PERMANENT,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
