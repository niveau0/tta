/*
 * Function name: sneak 
 * Description:   sneak somewhere
 * Arguments:     str - what or where to sneak
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sneak(string dir, object tp)
{
    int    i, sk, hide, percep, silence;
    object *listen;
    mixed  msg1, msg2;
    
    if (!dir)
    {
	tp->catch_tell("Usage: sneak <direction>\n");
	return -1;
    }
    switch (dir)
    {
      case "n":
	dir = "north";
	break;
      case "s":
	dir = "south";
	break;
      case "e":
	dir = "east";
	break;
      case "w":
	dir = "west";
	break;
      case "ne":
	dir = "northeast";
	break;
      case "nw":
	dir = "northwest";
	break;
      case "se":
	dir = "southeast";
	break;
      case "sw":
	dir = "southwest";
	break;
    }
    tp->add_prop(LIVE_WALKMODE, MOVE_SNEAK);
    if (go(dir, tp) == -1)
    {
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	return -1;
    }
    tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
    
    hide = tp->query_prop(OBJ_HIDE);
    sk = tp->skill_check(SK_STALK, 0, 0) + random(hide);
    listen = LISTEN(E(tp)) - ({ tp });
    silence = tp->query_prop(LIVE_NOISE_REDUCTION);
    if (silence / 1000 > 20)
	sk += 20;
    else
	sk += silence / 1000;
    /* people around may notice the sneak attempt */
    msg1 = ({ "You notice ", QNAME(tp), " sneaking " + dir + ".\n" });
    msg2 = ({ "You notice something's sneaking around here.\n" });
    for (i=sizeof(listen); i--;)
    {
	if (listen[i]->skill_check(SK_PERCEPTION, -sk, 0) > 0)
	{
	    if (VISIBLE(listen[i], tp))
		listen[i]->catch_msg(msg1, 0, 0);
	    else
		listen[i]->catch_msg(msg2, 0, 0);
	}
    }
    return 1;
}
