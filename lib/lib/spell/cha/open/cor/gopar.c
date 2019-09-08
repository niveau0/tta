inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <config.h>
#include <properties.h>
#include <state.h>
#include <event.h>

string
query_desc()
{
    return "This spell is useful at nights ambush. The magic awakes someone " +
	"without penalties someone receives normally after awakening " +
	"from sleep. Power manipulation has no effect.";
}

void
message(object tp, object *tar)
{
    if (tar[0] == tp)
	tp->catch_tell("Seems you are already awake.\n");
    else
	::message(tp, tar);
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar) && tar[0] != tp)
    {
	if (tar[0]->query_state() & (SLEEPING | MEDITATING))
	{
	    set_this_player(tar[0]);
	    MISC_SOUL->wake(0, tar[0], 1);
	    set_this_player(tp);
	}
    }
}

int
query_level()
{
    return 10;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_NONE,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
