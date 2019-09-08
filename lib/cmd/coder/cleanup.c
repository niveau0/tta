/* **************************************************************************
 * cleanup - cleanup a room -> remove objects
 */
nomask int
cleanup(string arg)
{
    int    i, save, liv, no_get;
    string *opts;
    object tp, *inv;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    inv = I(E(tp), 0);
    if (arg)
    {
	opts = explode(arg, " ");
	for (i=sizeof(opts); i--;)
	{
	    switch (opts[i])
	    {
	      case "-s":
		  save = 1;
		  break;
	      case "-l":
		  liv = 1;
		  break;
	      case "-f":
		  no_get = 1;
		  break;
	      default:
		  tp->catch_tell("Usage: cleanup [-s] [-l] [-f]\n");
		  return -1;
	    }
	}
    }
    for (i=sizeof(inv); i--;)
    {
	if (inv[i]->query_door_id())
	    continue;
	if (living(inv[i]) && (!liv || !inv[i]->query_npc()))
	    continue;
	if (inv[i]->query_prop(OBJ_NO_GET) && !no_get)
	    continue;
	remove_object(inv[i]);
    }
    if (save)
    {
	E(tp)->save();
	tp->catch_tell("Room cleaned and saved.\n");
    }
    else
	tp->catch_tell("Room cleaned but not saved.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " cleans the surroundings with a simple " +
			"gesture.\n"}), MSG_SEE);
    return 1;
}
