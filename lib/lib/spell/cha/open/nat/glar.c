inherit "../../cha_base";

#include <herb.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "This spells helps to understand the nature and origin "+
	"of herbs.";
}

void
spell_effect(int level, object *tar, object tp)
{
    mapping st;
    
    if (sizeof(tar))
    {
	if (tar[0]->query_prop(IS_HERB))
	{
	    st = tar[0]->query_status();
	    st[tp] = ITEM_ANALYSED;
	    tar[0]->set_status(st);
	    tp->catch_tell("You find out: It's " +
			   tar[0]->query_herb_name() + "\n");
	}
	else
	    tp->catch_tell("This kind of magic works only for herbs.\n");
	    
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
	    SP_PASSIVE,
	    SP_NONE,
	    120,
	    1,
	    SP_ALL_OBJECTS,
	    SK_ML_XZULOU,
	});
}
