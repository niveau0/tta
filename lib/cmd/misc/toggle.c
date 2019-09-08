/*
 * Function name: toggle
 * Description:   toggle different things on and off
 * Arguments:     str - what to toggle
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
toggle(string str, object tp)
{
    int l;
    
    if (str && sscanf(str, "pager %d", l))
    {
	if (l < 5)
	    l = 5;
	tp->add_prop(PLAYER_MORE_LEN, l);
	tp->echo("Pager chunk set to " + l + " now.\n");
	return 1;
    }
    switch (str)
    {
      case "attack":
	  if (!tp->query_prop(LIVE_NO_AUTOATTACK))
	  {
	      tp->add_prop(LIVE_NO_AUTOATTACK, 1);
	      tp->echo("Attacking switched on.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_NO_AUTOATTACK, 0);
	      tp->echo("Attacking switched off.\n");
	  }
	  return 1;
      case "brief":
	  if (!tp->query_prop(LIVE_SEE_SHORT_DESC))
	  {
	      tp->add_prop(LIVE_SEE_SHORT_DESC, 1);
	      tp->echo("You will only take a brief look at your " +
		       "surroundings.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_SEE_SHORT_DESC, 0);
	      tp->echo("You will take a closer look at your surroundings.\n");
	  }
	  return 1;
      case "drink":
	  if (!tp->query_prop(LIVE_AUTO_DRINK))
	  {
	      tp->add_prop(LIVE_AUTO_DRINK, 1);
	      tp->echo("You will try to drink something automatically if "+
		       "you are thirsty.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_AUTO_DRINK, 0);
	      tp->echo("You won't try to drink something automatically if "+
		       "you are thirsty.\n");
	  }
	  return 1;
      case "eat":
	  if (!tp->query_prop(LIVE_AUTO_EAT))
	  {
	      tp->add_prop(LIVE_AUTO_EAT, 1);
	      tp->echo("You will try to eat something automatically if "+
		       "you are hungry.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_AUTO_EAT, 0);
	      tp->echo("You won't try to eat something automatically if "+
		       "you are hungry.\n");
	  }
	  return 1;
      case "echo":
	  if (!tp->query_prop(LIVE_GET_ECHO))
	  {
	      tp->add_prop(LIVE_GET_ECHO, 1);
	      tp->catch_tell("Echo switched on.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_GET_ECHO, 0);
	      tp->catch_tell("Echo switches off.\n");
	  }
	  return 1;
      case "fights":
	  if (!tp->query_prop(LIVE_SEE_FIGHT))
	  {
	      tp->add_prop(LIVE_SEE_FIGHT, 1);
	      tp->echo("You will keep your eyes open on " +
		       "other fights.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_SEE_FIGHT, 0);
	      tp->echo("You won't look at other fights.\n");
	  }
	  return 1;
      case "inventory":
	  if (!tp->query_prop(LIVE_LONG_INVENTORY))
	  {
	      tp->add_prop(LIVE_LONG_INVENTORY, 1);
	      tp->echo("You will see your inventory more detailed now.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_LONG_INVENTORY, 0);
	      tp->echo("You will see your inventory less detailed now.\n");
	  }
	  return 1;
      case "trade":
	  if (!tp->query_prop(LIVE_TRADING))
	  {
	      tp->add_prop(LIVE_TRADING, 1);
	      tp->echo("You will try to lower the price now.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_TRADING, 0);
	      tp->echo("You won't try to trade anymore.\n");
	  }
	  return 1;
      case "walking":
	  if (!tp->query_prop(LIVE_SEE_NO_WALKING))
	  {
	      tp->add_prop(LIVE_SEE_NO_WALKING, 1);
	      tp->echo("You won't get walking messages.\n");
	  }
	  else
	  {
	      tp->add_prop(LIVE_SEE_NO_WALKING, 0);
	      tp->echo("You will get walking messages.\n");
	  }
	  return 1;
      default:
	  tp->catch_tell("Usage: toggle attack|brief|drink|eat|echo|fights|"+
			 "inventory|walking|trade\n" +
			 "       toggle pager <number>\n" +
			 "- autoattack    is " +
			 (!tp->query_prop(LIVE_NO_AUTOATTACK) ? "on" : "off") +
			 "\n- brief look    is " +
			 (tp->query_prop(LIVE_SEE_SHORT_DESC) ? "on" : "off") +
			 "\n- auto drink    is " +
			 (tp->query_prop(LIVE_AUTO_DRINK) ? "on" : "off") +
			 "\n- auto eat      is " +
			 (tp->query_prop(LIVE_AUTO_EAT) ? "on" : "off") +
			 "\n- echo          is " +
			 (tp->query_prop(LIVE_GET_ECHO) ? "on" : "off") +
			 "\n- watch fights  is " +
			 (tp->query_prop(LIVE_SEE_FIGHT) ? "on" : "off") +
			 "\n- inventory     is shown " +
			 (tp->query_prop(LIVE_LONG_INVENTORY) ?
			  "detailed" : "short") +
			 "\n- trading       is " +
			 (tp->query_prop(LIVE_TRADING) ? "on" : "off") +
			 "\n- watch walking is " +
			 (!tp->query_prop(LIVE_SEE_NO_WALKING) ? "on" :"off") +
			 "\n- pagechunk     is " +
			 tp->query_prop(PLAYER_MORE_LEN) +
			 "\n");
	  return -1;
    }
}
