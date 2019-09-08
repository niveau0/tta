inherit "../../ess_base";

#include <composite.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <math.h>
#include <skills.h>

string
query_desc()
{
    return "Caster can tell the exact mass and weight of one object.";
}


void
spell_effect(int level, object *tar, object tp, string extra)
{
    if ( sizeof(tar))
    {
	tell_objects(({ tp }),
		     ({ "You concentrate on " , QTNAME(tar[0]),
			    " and find out, that it weights " ,
			    LANG_NUMW(tar[0]->query_weight()),
			    " grams and is ",
			    MATH_INCHES_TO_STRING(tar[0]->query_size()),
			    " tall.\n" }), MSG_SEE);
    }
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
	    SP_INSTANTLY,
	    SP_NONE,
	    SP_TOUCH,
	    1,
	    SP_ALL_TARGETS,
	    SK_ML_CONSCIOUSNESS,
	});
}
