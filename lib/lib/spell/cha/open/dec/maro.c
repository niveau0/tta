inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect active spells and realm on someone/something.";
}

int
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int    i;
	string str, list, name, realm;
	object *obj;

	obj = tar[0]->query_spells();
	str = "";
	for (i=sizeof(obj); i--;)
	{
	    if (sscanf(MASTER_OB(obj[i]), SPELL_DIR + "/%s/%*s/%s/%s",
		       realm, list, name) != 4)
		continue;
	    str += "* '" + list + " " + name + "' of the magic realm '" +
		   realm + "', powerlevel " +
		   obj[i]->query_cast_level() + "\n";
			   
	}
	if (str == "")
	    tp->catch_tell("You discover no spells.\n");
	else
	    tp->catch_tell("You sense the following spells:\n" + str);
    }
    return 1;
}

int
query_level()
{
    return 15;
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
	    SP_ALL_TARGETS,
	    SK_ML_CONSCIOUSNESS,
	});
}
