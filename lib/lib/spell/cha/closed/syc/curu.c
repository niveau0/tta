inherit "../../cha_base";

#include <config.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

#define CAST_LEVEL "_cast_level"

void start_ask(object tp);

string
query_desc()
{
    return "Embeds a symbol on the floor. The symbol reacts if touched. " +
	"Power manipulates the level of the spell that can be written as " +
	"a symbol.";
}

void
spell_effect(int level, int *co, object tp)
{
    tp->catch_tell("You point at your chosen area.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " points at the ground.\n" }), MSG_SEE, tp);
    tp->add_prop(CAST_LEVEL, level);
    start_ask(tp);
}

void
start_ask(object tp)
{
    set_this_player(tp);
    tp->catch_tell("\nWhat spell do you wish to emplace?\nPlease input the " +
		   "spell ('<power> <spell> <realm> <list>'):\n");
    input_to("spell_name");
}

static void
spell_name(string str)
{
    int    clev;
    object tp;
    mixed  args, data;
    
    tp = this_player();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled.\n");
	tp->add_prop(CAST_LEVEL, 0);
	return;
    }
    if ((args = MISC_SOUL->parse_cast(str)) == -1)
    {
	start_ask(tp);
	return;
    }
    clev = tp->query_prop(CAST_LEVEL);
    if (args[0]->query_level() > 1 || args[1] > clev / 5)
    {
	tp->catch_tell("That spell is too powerful.\n");
	start_ask(tp);
	return;
    }
    data = args[0]->query_spell_data();
    if (data[SP_DURATION] == SP_CONCENTRATE)
    {
	tp->catch_tell("You failed to emplace that spell.\n");
	return;
    }
    // quick bug fix, needs to be corrected
    if (!test_success(tp, 1, data[SP_TYPE], 0, 0))
	return;
    
    E(tp)->
	add_symbol(tp, tp->query_prop(CAST_LEVEL), args[0], 1, args[4]);
    tp->add_prop(CAST_LEVEL, 0);
}

int
query_level()
{
    return 5;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_PERMANENT,
	    120,
	    SP_DIRECTION,
	    0,
	    SK_ML_GRAPHAY,
	});
}
