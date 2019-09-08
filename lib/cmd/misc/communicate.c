/*
 * Function name: speak
 * Description:   say something with a special voice
 * Arguments:     how - how you say it
 *                str - string to say
 *                dist - hearing distance
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
static nomask int
speak(string how, string str, int dist, object tp)
{
    int    i, sz, lang, *letters;
    string mod;
    object *listen;
    mixed  msg;
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: " + how + " <text>\n");
	return -1;
    }
    if ((i = strlen(str)) && str[i-1] == '?')
	how = "ask";
	
    tp->echo("You " + how + ": " + str + "\n");
    
    if (!(sz = sizeof(listen = LISTEN(E(tp)) - ({ tp }))))
	return 1;
    
    lang = tp->query_prop(LIVE_USED_LANGUAGE);
    if (!(letters = langletters[lang]))
	letters = langletters[SK_WESTRON];
    mod = modify_language(lang, str, letters, tp);
    msg = ({ QCTNAME(tp),
	     " utters something totally incomprehensible.\n" });
    if (!mod) /* speaker has no language skill */
    {
	for (i=0; i<sz; i++)
	    listen[i]->catch_msg(msg, tp, MSG_HEAR, dist);
    }
    else
    {
	mixed msg2;
	
	msg2 = ({ QCTNAME(tp), " " + how + "s in " +
		  capitalize(get_language(lang)) + ": ", 0, "\n" });
	for (i=0; i<sz; i++)
	{
	    if (!(msg2[2] = modify_language(lang, mod, letters, listen[i])))
		listen[i]->catch_msg(msg, tp, MSG_HEAR, dist);
	    else
	    {
		if (!random(10))
		    listen[i]->skill_check(lang, 0, 0);
		listen[i]->catch_msg(msg2, tp, MSG_HEAR, dist);
	    }
	}
    }
    return 1;
}

/*
 * Function name: commanding
 * Description:   command someone to do something
 * Arguments:     str - who and what to do
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
commanding(string str, object tp)
{
    string cmd;
    object ob, *list;
    
    if (!str || sscanf(str, "%s to %s", str, cmd) != 2)
    {
	tp->catch_tell("Usage: command <whom> to <cmd>\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 2));
    list = parse(str, list, tp);
    if (!check_size(list, "No '" + str + "' to command here.\n", tp))
	return -1;
    ob = list[0];
    speak("command", str + "! " + cmd, 1920, tp); /* 160 feet */
    if (ob->query_prop(LIVE_CONTROLLED_BY) == tp && CAN_HEAR(ob, tp, 1900))
	ob->command(cmd);
    return 1;
}


/*
 * Function name: talk
 * Description:   say a long sentence at once
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
talk(string str, object tp)
{
    tp->catch_tell("Enter text to say, exit with '**'\n]");
    input_to("talk_next", 0, "");
    return 1;
}

static nomask void
talk_next(string str, string text)
{
    int    i, sz, lang, *letters;
    string mod;
    object tp, *listen;
    mixed  msg;
	
    tp = this_player();
    if (str == "**")
    {
	tp->catch_tell("Ok.\n");
	if (!(sz = sizeof(listen = LISTEN(E(tp)) - ({ tp }))))
	    return;
	lang = tp->query_prop(LIVE_USED_LANGUAGE);
	if (!(letters = langletters[lang]))
	    letters = langletters[SK_WESTRON];
	mod = modify_language(lang, text, letters, tp);
	msg = ({ QCTNAME(tp),
		 " utters something totally incomprehensible.\n" });
	if (!mod) /* talking living has no language skill */
	{
	    for (i=0; i<sz; i++)
		listen[i]->catch_msg(msg, tp, MSG_HEAR, 590);
	}
	else
	{
	    mixed msg2, msg3;
	    
	    msg2 = ({ QCTNAME(tp), " says in " +
		      capitalize(get_language(lang)) + ":\n" });
	    for (i=0; i<sz; i++)
	    {
		if (!(msg3 = modify_language(lang, mod, letters, listen[i])))
		    listen[i]->catch_msg(msg, tp, MSG_HEAR, 590);
		else
		{
		    listen[i]->catch_msg(msg2, tp, MSG_HEAR, 590, 1);
		    listen[i]->catch_tell(msg3);
		}
	    }
	}
	return;
    }
    if (!str)
        text += "\n";
    else
        text += str + "\n";
    tp->catch_tell("]", 1);
    input_to("talk_next", 0, text);
}

/*
 * Function name: grumble
 * Description:   grumble something
 * Arguments:     str - string to grumble
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
grumble(string str, object tp)
{
    return speak("grumble", str, 590, tp); /* 15 meters */ 
}

/*
 * Function name: mumble
 * Description:   mumble something
 * Arguments:     str - string to mumble
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
mumble(string str, object tp)
{
    return speak("mumble", str, 200, tp); /* 5 meters */
}

/*
 * Function name: say
 * Description:   say something
 * Arguments:     str - string to say
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
say(string str, object tp)
{
    return speak("say", str, 590, tp); /* 15 meters */
}

/*
 * Function name: shout
 * Description:   shout something
 * Arguments:     str - string to shout
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
shout(string str, object tp)
{
    return speak("shout", str, 2000, tp); /* 50 meters */
}

/*
 * Function name: shriek
 * Description:   shriek something
 * Arguments:     str - string to shriek
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
shriek(string str, object tp)
{
    return speak("shriek", str, 1500, tp); /* 40 meters */
}

/*
 * Function name: sing
 * Description:   sing something
 * Arguments:     str - string to sing
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sing(string str, object tp)
{
    if (roll_dice(-1) + tp->query_skill_bonus(SK_SINGING) < 100)
    {
	tp->command("caw");
	return 1;
    }
    return speak("sing", str, 590, tp); /* 15 meters */
}

/*
 * Function name: whisper
 * Description:   whisper something to someone
 * Arguments:     str - the string to whisper
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
whisper(string str, object tp)
{
    int    lang, *letters;
    string whom, mod;
    object *listen, *list;
    mixed  msg;

    if (!str)
    {
	tp->catch_tell("Whisper <what> [to <whom>]?\n");
	return -1;
    }
    if (sscanf(str, "%s to %s", str, whom) != 2)
	return speak("whisper", str, 80, tp); /* 2 meters */
    list = tp->visible(listen = LISTEN(E(tp)));
    list = parse(whom, list, tp, tp->query_step());
    if (!check_size(list, "No '" + whom + "' in range to whisper to.\n", tp))
	return -1;
    if (tp == list[0])
    {
	tp->catch_tell("Whisper to yourself???\n");
	return -1;
    }
    tp->echo("You whisper '" + str + "' to " + whom + ".\n");
    
    lang = tp->query_prop(LIVE_USED_LANGUAGE);
    if (!(letters = langletters[lang]))
	letters = langletters[SK_WESTRON];
    mod = modify_language(lang, str, letters, tp);
    if (!mod || !(mod = modify_language(lang, mod, letters, list[0])))
    {
	msg = ({ QCTNAME(tp),
		 " utters something totally incomprehensible.\n" });
    }
    else
    {
	msg = ({ QCTNAME(tp), " whispers in " +
		 capitalize(get_language(lang)) + " to you: " +
		 mod + "\n" });
    }
    list[0]->catch_msg(msg, tp, MSG_HEAR, 96); /* 8 feet */
    /* whispering can only been seen */
    tell_objects(listen - ({ tp, list[0] }),
		 ({ QCTNAME(tp), " whispers something to ",
			QTNAME(list[0]), ".\n" }), MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
