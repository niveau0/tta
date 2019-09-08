/* **************************************************************************
 * set_teleport_out - set teleport out message.
 */
nomask int
setmsg(string m)
{
    string type;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!m || sscanf(m, "%s %s", type, m) != 2)
    {
	tp->catch_tell("Usage: setmsg " +
		       "[mArrive|mLeave|walk|run] <message>\n");
	return -1;
    }
    type = lower_case(type);
    switch (type)
    {
      case "marrive": tp->set_teleport_in(m); break;
      case "mleave":  tp->set_teleport_out(m); break;
      case "walk":    tp->set_move_msg(m); break;
      case "run":     tp->set_run_msg(m); break;
      default:
	  tp->catch_tell("Usage: setmsg " +
			 "[mArrive|mLeave|walk|run] <message>\n");
	  return -1;
    }
    return 1;
}
