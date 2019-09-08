inherit "../../cha_base";

#include <herb.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <macros.h>

string
query_desc()
{
    return "This spell will help you to understand more about the flora.";
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    if(sizeof(tar))
    {
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 5 * level, level);
	tar[0]->add_spell(ob);
	tar[0]->add_tmp_skill(SK_FLORA_LORE, level);

	if(tar[0] == tp)
	{
	    tp->catch_tell("You feel wisdom about the flora rising in your "+
			   "mind.\n");
	    return;
	}
	tar[0]->catch_tell("You feel wisdom about the flora rising in your "+
			   "mind.\n");
	tp->catch_msg(({"You make ", QTNAME(tar[0]), " wiser concerning the ",
			    "lore of the flora.\n"}), 0, MSG_SEE);
	return;
    }
    tp->catch_msg("Something failed in your spell.\n");
    return;
}

void
remove_spell(int level)
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("You feel your knowledge about "+
			   "the flora fading.\n");
	tar[0]->add_tmp_skill(SK_FLORA_LORE, -level);
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
	    SP_PASSIVE,
	    60,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_XZULOU,
	});
}
