/*
 * Function name: sla
 * Description:   toggle spatial location awareness usage
 * Arguments:     str - turn on/off
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sla(string str, object tp)
{
    switch (str)
    {
      case "on":
	  if (tp->query_prop(LIVE_USING_SLA))
	  {
	      tp->catch_tell("You already using your sla skill.\n");
	      return 1;
	  }
	  tp->add_prop(LIVE_USING_SLA, 1);
	  tp->echo("You close your eyes and concentrate.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " closes the eyes and frowns.\n" }),
		       MSG_SEE, tp);
	  return 1;
      case "off":
	  if (!tp->query_prop(LIVE_USING_SLA))
	  {
	      tp->catch_tell("You already have your eyes open.\n");
	      return 1;
	  }
	  tp->add_prop(LIVE_USING_SLA, 0);
	  tp->echo("You open your eyes.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " opens the eyes.\n" }),
		       MSG_SEE, tp);
	  return 1;
      default:
	  tp->catch_tell("Usage: sla on|off\n");
	  return -1;
    }
}
