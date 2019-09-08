/*
 * player generation
 * This object queries the player for attributes and the look he/she
 * would like for the new character. The player him/herself will be
 * created by the coders of the game.
 */

#include <macros.h>
#include <magic.h>
#include <race.h>
#include <config.h>
#include "generation.h"

static nomask void
create()
{
    if (IS_CLONE)
	destruct_object();
}

static nomask void
quit(object ob)
{
    ob->catch_tell("See you later. Bye.\n");
    destruct_object(ob);
}

/*
 * Function name: generation
 * Description:   called from login object to start the generation
 * Arguments:     ob - the login object
 */
nomask void
generation(object ob, string name)
{
    mapping data;

    if (!ob || MASTER_OB(ob) != _LOGIN)
	return;

    /* default realm is divine, only with magic
     * talent you can choose another
     */
    data = ([ GEN_NAME:lower_case(name), GEN_REALM:CHA, ]);

    ob->catch_tell("\n\n\nWelcome traveller. You are about to visit a " +
		   "virtual world. Following\nnow is a short \"we question, " +
		   "you answer\" procedure to find out what kind of\n" +
		   "character you like to play.\nYou could use the question " +
		   "mark '?' to get help at every stage, or 'quit'\nto " +
		   "leave.\nYou may also create your character via our " +
		   "webpage (see above).\n\nOk, first the essential " +
		   "question. What is your email address (you\n"+
		   "need an email to play a character here)?\n");
    set_this_player(ob);
    input_to("email", 0, ob, data);
}

static void
email(string str, object ob, mapping data)
{
    if (!ob)
	return;
    
    if (str == "?")
    {
	ob->catch_tell("The email address is needed so we can let you know " +
		       "that your character is\nready to play. Also " +
		       "the email is your account for different " +
		       "characters here.\nSo what is your email?\n");
	input_to("email", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str && sscanf(str, "%*s@%*s.%*s") == 3)
    {
	data[GEN_EMAIL] = str;
	ob->catch_tell("The following races can be chosen:\n" +
		       implode(RACEGENERAL, " - ") + "\nNow choose a race.\n");
	input_to("race", 0, ob, data);
	return;
    }
    ob->catch_tell("This seems to be no valid email. What is your email?\n");
    input_to("email", 0, ob, data);
}

static nomask void
subrace(string str, object ob, mapping data)
{
    string race;
    
    if (str == "?")
    {
	ob->catch_tell("Use ? <racename> to get info about a special race " +
		       "here.\nThe following races can be chosen:\n" +
		       implode(SUBRACE[data[GEN_RACE]], " - ") + "\n");
	input_to("subrace", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str && sscanf(str, "? %s", race) && IN_ARRAY(race, RACES))
    {
	ob->catch_tell(RACEDESC[race] + "Now choose a race.\n");
	input_to("subrace", 0, ob, data);
	return;
    }
    if (str && IN_ARRAY(str, RACES))
    {
	data[GEN_RACE] = str;
	ob->catch_tell("Now please write down a small description how your " +
		       "character should look like\nwhen first met.  " +
		       "This should not exceed 30 letters if possible " +
		       "(e.g.\ngreen eyed, weathered halfelf)\n");
	input_to("desc", 0, ob, data);
	return;
    }
    ob->catch_tell("This is no valid race. Please choose again.\n");
    input_to("subrace", 0, ob, data);
}

static nomask void
race(string str, object ob, mapping data)
{
    string race;
    
    if (str == "?")
    {
	ob->catch_tell("Use ? <racename> to get info about a special race " +
		       "here.\nThe following races can be chosen:\n" +
		       implode(RACEGENERAL, " - ") + "\n");
	input_to("race", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str && sscanf(str, "? %s", race) && IN_ARRAY(race, RACEGENERAL))
    {
	ob->catch_tell(RACEDESC[race] + "Of which race do you wish to be?\n");
	input_to("race", 0, ob, data);
	return;
    }
    if (str && IN_ARRAY(str, RACEGENERAL))
    {
	data[GEN_RACE] = str;
	if (SUBRACE[str])
	{
	    ob->catch_tell("There are different kinds of that race.\n" +
			   implode(SUBRACE[str], " - ") +
			   "\nPlease choose one of those.\n");
	    input_to("subrace", 0, ob, data);
	    return;
	}
	subrace(str, ob, data);
	return;
    }
    ob->catch_tell("This is no valid race. Please choose again.\n");
    input_to("race", 0, ob, data);
}

static nomask void
desc(string str, object ob, mapping data)
{
    if (str == "?")
    {
	ob->catch_tell("The description should contain one or two " +
		       "adjectives, but its looks confusing for others " +
		       "if your description is too long\n");
	input_to("desc", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (!str || !strlen(str))
    {
	ob->catch_tell("That's no valid description.\nPlease input " +
		       "another description:\n");
	input_to("desc", 0, ob, data);
	return;
    }
    if (strlen(str) < 50)
    {
	data[GEN_DESC] = str;
	ob->catch_tell("Ok. Choose the sex of your character (male or " +
		       "female):\n");
	input_to("gender", 0, ob, data);
	return;
    }
    ob->catch_tell("That's too much of a short description.\nPlease " +
		   "input another _short_ description:\n");
    input_to("desc", 0, ob, data);
}

static nomask void
gender(string str, object ob, mapping data)
{
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str != "male" && str != "female")
    {
	ob->catch_tell("This is no valid gender. Choose again (male " +
		       "or female):\n");
	input_to("gender", 0, ob, data);
	return;
    }
    data[GEN_GENDER] = str;
    ob->catch_tell("Now you have to choose which talent your character\n"+\
		   "should have. Choose one of the following:\n" +
		   implode(TALENTS, " - ") + "\n");
    input_to("talent", 0, ob, data);
}

static nomask void
talent(string str, object ob, mapping data)
{
    if (str == "?")
    {
	ob->catch_tell("Your talent will be the general range where you " +
		       "could raise your wisdom\neasier. For example if " +
		       "you are an artist it will be easier for you to " +
		       "learn\nplaying an instrument, if your have magic " +
		       "talent, its easier to\nlearn spells and magic " +
		       "lore. Please choose:\n" +
		       implode(TALENTS, " - ") + "\n");
	input_to("talent", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str && IN_ARRAY(str, TALENTS))
    {
	data[GEN_TALENT] = str;
	if (str == "magic")
	{
	    ob->catch_tell("Oh, with magic talent you are free to choose " +
			   "which magic realm you are\nassociated with. The " +
			   "following realms exist, choose one of those. " +
			   "Use '? <realm>'\n to get some " +
			   "more information.\n" + implode(REALMS, " - ") +
			   "\n");
	    input_to("realm", 0, ob, data);
	    return;
	}
	ob->catch_tell("Do you like to start with an (e)xperienced " +
		       "character, or do you wish to start\nyour career " +
		       "from the (b)eginning?\n");
	input_to("career", 0, ob, data);
	return;
    }
    ob->catch_tell("Wrong talent. Choose one of:\n" +
		   implode(TALENTS, " - ") + "\n");
    input_to("talent", 0, ob, data);
}

static nomask void
realm(string str, object ob, mapping data)
{
    string realm;
    
    if (str == "?")
    {
	ob->catch_tell("Use: ? <realm>\n");
	input_to("realm", 0, ob, data);
	return;
    }
    if (str && sscanf(str, "? %s", realm) && IN_ARRAY(realm, REALMS))
    {
	ob->catch_tell(REALMDESC[realm]);
	input_to("realm", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str && IN_ARRAY(str, REALMS))
    {
	data[GEN_REALM] = str;
	ob->catch_tell("Do you like to start with an (e)xperienced " +
		       "character, or do you wish to start\nyour career " +
		       "from the (b)eginning?\n");
	input_to("career", 0, ob, data);
	return;
    }
    ob->catch_tell("That's none of the valid realms.\n" +
		   implode(REALMS, " - ") + "\nChoose again.\n");
    input_to("realm", 0, ob, data);
    return;
}

static nomask void
career(string str, object ob, mapping data)
{
    string out;
    
    if (str == "?")
    {
	ob->catch_tell("If you choose an experienced character you have " +
		       "some _possible_ advantages\nalready. Some more " +
		       "equipment, a job in your hometown, some experience " +
		       "in\nskills. But there is the disadvantage that the " +
		       "development of your character\n(since your birth up " +
		       "to the current time) layed in the hands of the " +
		       "gods, so\nyou will have no influence on what you " +
		       "have learned already.\n" +
		       "So choose (e)xperienced or (b)eginner:\n");
	input_to("career", 0, ob, data);
	return;
    }
    if (str == "quit")
    {
	quit(ob);
	return;
    }
    if (str == "e" || str == "b")
    {
	data[GEN_CAREER] = str;
	ob->catch_tell("\n\n\nThat's it.\n" +
		       "If your character is accepted you will " +
		       "receive a mail with your\npassword.\n" +
		       "See you then.\n\n\n");
	debug("manwe", "new application");
	CREATE_PLAYER->new_player(data);
	destruct_object(ob);
	return;
    }
    ob->catch_tell("Wrong value, choose 'e' to start experienced, or 'b' " +
		   "for beginner.\n");
    input_to("career", 0, ob, data);
}
