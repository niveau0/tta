inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Tears the soul from a targets body.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
	tar[0]->add_unconscious(tar[0]->query_prop(LIVE_LAST_RR_RESULT) *
				7000);
}

int
query_rr_mod()
{
    return -20;
}

int
query_level()
{
    return 14;
}

int *
query_spell_data()
{
    return
	({
	    (SP_FORCE | SP_INSTANTLY),
	    SP_NONE,
	    600,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_QADOSH,
	});
}
