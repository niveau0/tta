/*
 * Function name: language
 * Description:   choose language
 * Arguments:     str - the language to speak now
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
language(string str, object tp)
{
    string *lang;
    
    if (!str)
    {
	str = get_language(tp->query_prop(LIVE_USED_LANGUAGE));
	if (str == "")
	    str = "senseless";
	tp->catch_tell("The following languages are spoken on Arda:\n"+
		       "Westron      (most common language)\n"+
		       "Roheric      (mainly spoken in Rohan)\n"+
		       "Adunaic      (old numenorean language)\n"+
		       "Quenya       (old lost elvish tongue)\n"+
		       "Sindarin     (main tongue of the elves)\n"+
		       "Khuzdul      (spoken by the dwarves)\n"+
		       "Black speech (the tongue of Mordor and evil)\n" +
		       "You currently speak: " + str + ".\n");
	return -1;
    }
    str = lower_case(str);
    switch (str)
    {
      case "westron":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
	break;
      case "roheric":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_ROHERIC);
	break;
      case "adunaic":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_ADUNAIC);
	break;
      case "quenya":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_QUENYA);
	break;
      case "sindarin":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_SINDARIN);
	break;
      case "khuzdul":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_KHUZDUL);
	break;
      case "black speech":
	tp->add_prop(LIVE_USED_LANGUAGE, SK_BLACK_SPEECH);
	break;
      default:
	tp->catch_tell("This is not a known language.\n");
	return -1;
    }
    tp->echo("You switched to " + str + ".\n");
    return 1;
}
