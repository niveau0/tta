/*
 * Function name: mood
 * Description:   show current or set the fighting mood
 * Arguments:     str - new mood 
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
mood(string str, object tp)
{
    int    mood;

    mood = tp->query_prop(LIVE_FIGHT_MOOD);
    if (!str)
    {
	switch (mood)
	{
	  case 0:
	    str = "wimpy";
	    break;
	  case 25:
	    str = "careful";
	    break;
	  case 50:
	    str = "balanced";
	    break;
	  case 75:
	    str = "brave";
	    break;
	  case 100:
	    str = "aggressive";
	    break;
	  case 120:
	    str = "frenzy";
	}
	tp->catch_tell("Your current fighting mood is " + str + ".\n");
	return 1;
    }
    if (mood == 120 && str != "frenzy")
    {
	if (tp->skill_check(SK_FRENZY, -100, 0) > 0)
	{
	    tp->catch_tell("You fail to calm yourself.\n");
	    return -1;
	}
    }
    else if (mood != 120 && str == "frenzy")
    {
	if (tp->skill_check(SK_FRENZY, -100, 0) > 0)
	{
	    tp->catch_tell("You fail to set yourself into berserk mood.\n");
	    return -1;
	}
    }
    switch (str)
    {
      case "wimpy":
	mood = 0;
	break;
      case "careful":
	mood = 25;
	break;
      case "balanced":
	mood = 50;
	break;
      case "brave":
	mood = 75;
	break;
      case "aggressive":
	mood = 100;
	break;
      case "frenzy":
	mood = 120;
	break;
      default:
	tp->catch_tell("No valid mood. Possible are:\nwimpy, careful, " +
		       "balanced, brave, aggressive and frenzy.\n");
	return -1;
    }
    tp->add_prop(LIVE_FIGHT_MOOD, mood);
    tp->echo("Your current fighting mood is now " + str + ".\n");
    return 1;
}
