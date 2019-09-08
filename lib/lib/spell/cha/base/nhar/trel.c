inherit "../../cha_base";

#include <config.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

static object receiver;

string
query_desc()
{
    return "This enables the caster to channel another spell to a target " +
	"so the receiver is able to cast the channeled spell.";
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;

    ob = clone_object(MASTER);
    ob->store_spell(tp, level, 0);
    tp->add_spell(ob);
    ob->start_ask(tp);
}

void
start_ask(object tp)
{
    set_this_player(tp);
    tp->catch_tell("Who should receive the spell? <cr> to cancel.\n");
    input_to("answer_target");
}

void
remove_spell()
{
    destruct_object();
}

static void
answer_target(string str)
{
    int    i, sz;
    object tp, *u;

    tp = this_player();
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled.\n");
	destruct_object();
	return;
    }
    str = lower_case(str);
    u = users() - ({ tp });
    for (i=0, sz=sizeof(u); i<sz; i++)
    {
	if (tp->query_met(u[i]->query_name()) == str)
	    break;
    }
    if (i == sz || !(u[i]->query_realm() & tp->query_realm()))
    {
	tp->catch_tell("You cannot sense anyone called '" + str +
		       "'.\nWho should receive the spell? " +
		       "<cr> to cancel.\n");
	input_to("answer_target");
	return;
    }
    receiver = u[i];
    tp->catch_tell("You must wait now for a mental answer...\n");
    receiver->catch_tell("A mental presence takes place in your mind. You " +
			 "notice that someone\nwants to send you a spell. " +
			 "If you want to open your mind to receive it,\nuse " +
			 "'receive spell'.\n");       
    receiver->define_primary("receive_spell", this_object());
    call_out("too_late", 120);
}

static void
too_late()
{
    if (query_caster())
	query_caster()->catch_tell("Suddenly the channel is lost.\n");
    if (receiver)
	receiver->catch_tell("Suddenly the channel is lost.\n");
    remove_spell();
}

int
receive_spell(string cmd, string arg)
{
    if (cmd != "receive" || arg != "spell")
	return 0;
    if (!query_caster() || receiver != this_player())
    { 
	too_late();
	return 1;
    }
    remove_call_out("too_late");
    receiver->undefine_primary("receive_spell", this_object());
    receiver->catch_tell("Ok, you open the channel.\n");
    query_caster()->catch_tell("The channel is open now.\n");
    call_out("get_spell", 0);
    return 1;
}

static void
get_spell()
{
    if (!query_caster())
    {
	too_late();
	return;
    }
    set_this_player(query_caster());
    query_caster()->catch_tell("\nWhat spell do you wish to channel\n"+
			       "(e.g. 'cha solr chan'): ");
    input_to("spell_name");
}

static void
spell_name(string str)
{
    int    chlev;
    object tp;
    mixed  args;
    
    tp = this_player();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled.\n");
	return;
    }
    if ((args = MISC_SOUL->parse_cast(str)) == -1)
    {
	get_spell();
	return;
    }
    chlev = query_cast_level() / 2;
    if (args[0]->query_level() > chlev || args[1] > chlev)
    {
	tp->catch_tell("You can only send " + LANG_WORD(chlev) +
		       " level spells.\n");
	return;
    }
    if (receiver)
    {
	string *parts;
	mixed  prop;
	
	parts = explode(str, " ");
	tp->catch_tell("Done.\n");
	receiver->catch_tell("You received the spell knowledge of '" +
			     (str = implode(parts[..1], " ")) + "'.\n");
	if (mappingp(prop = receiver->query_prop(LIVE_TMP_SPELLKNOWLEDGE)))
	    prop[args[0]] = chlev;
	receiver->add_prop(LIVE_TMP_SPELLKNOWLEDGE, prop);
    }
    too_late();
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
	    SP_PASSIVE | SP_INSTANTLY,
	    SP_NONE,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_QADOSH,
	});
}
