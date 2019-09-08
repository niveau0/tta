inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>
#include <state.h>

string
query_desc()
{
    return "Increase language a certain language skill as long as the " +
	"caster concentrates.";
}

mixed
query_similar_code()
{
    return "_speca_" + query_data();
}

varargs void
spell_effect(int level, object *tar, object tp, string extra)
{
    int    lang;
    
    switch (extra)
    {
      case "westron":
	lang = SK_WESTRON;
	break;
      case "roheric":
	lang = SK_ROHERIC;
	break;
      case "adunaic":
	lang = SK_ADUNAIC;
	break;
      case "quenya":
	lang = SK_QUENYA;
	break;
      case "sindarin":
	lang = SK_SINDARIN;
	break;
      case "khuzdul":
	lang = SK_KHUZDUL;
	break;
      case "black speech":
	lang = SK_BLACK_SPEECH;
	break;
      default:
	tp->catch_tell("This is not a known language.\n");
	return;
    }
    
    if (sizeof(tar))
    {
	object ob;
	
	tar[0]->catch_tell("You feel more familiar with " + extra + ".\n");
	
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar, lang);
	ob->start_concentrate(120);
	tp->add_spell(ob);
	if (tp != tar[0])
	    tar[0]->add_spell(ob);
	tar[0]->add_tmp_skill(lang, level * 3);
    }
}

void
remove_spell()
{
    object  c, *tar;

    if (c = query_caster())
    {
	c->delete_state(CONCENTRATE);
	c->add_prop(CONCENTRATE_DATA, 0);
    }
    
    if (sizeof(tar = query_targets()))
	tar[0]->add_tmp_skill(query_data(), -query_cast_level() * 3);
    destruct_object();
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
	    SP_CONCENTRATE,
	    120,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_LOGOS,
	});
}
