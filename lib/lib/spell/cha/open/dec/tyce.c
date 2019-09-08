inherit "../../cha_base";

#include <language.h>
#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <skills.h>

string
query_desc()
{
    return "Analyse a living body.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int    birth;
	string pron, age;
	object t;
    
	t = tar[0];
	pron = t->query_pronoun();
	if (!(birth = t->query_birth()))
	    age = "You failed to spot " + t->query_possessive() + " age.";
	else
	    age = (capitalize(pron) + " is " +
		   CONV_REALTIME((time() - birth) * TIME_SCALE) + " old.");
	
	tp->catch_tell(BS("This is " + LANG_ADDART(t->query_race()) +
			  ". " + age + " You sense that " + pron + " is " +
			  (100 - (t->query_hp() * 100 / t->query_max_hp())) +
			  "% damaged. The mental abilties are at " +
			  (t->query_hp() * 100 / t->query_max_hp()) +
			  "%.\n", 77, 0));
    }
}

int
query_level()
{
    return 8;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_NONE,
	    600,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_CONSCIOUSNESS,
	});
}
