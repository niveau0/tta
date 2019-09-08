/* **************************************************************************
 * invis - become invisible
 */
nomask int
invis()
{
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (tp->query_prop(OBJ_INVIS))
    {
	tp->catch_tell("You are already invisible.\n");
	return 1;
    }
    tp->catch_tell("You are now invisible.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " " + tp->query_teleport_out() + "\n" }),
		 MSG_SEE, tp);
    tp->change_int_prop(OBJ_INVIS, 1);
    return 1;
}
