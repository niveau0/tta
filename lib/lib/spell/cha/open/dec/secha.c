inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect active spells of the 'cha' realm.";
}

string
query_detect_realm()
{
    return "cha";
}

int
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int    i;
	string str, name, path;
	object *obj;

	path = SPELL_DIR + "/" + query_detect_realm() + "/%*s/%*s/";
	obj = tar[0]->query_spells();
	str = "";
	for (i=sizeof(obj); i--;)
	{
	    if (sscanf(MASTER_OB(obj[i]), path +"%s", name) != 3)
		continue;
	    str += "* '" + name + "'\n";
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
    return 1;
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
