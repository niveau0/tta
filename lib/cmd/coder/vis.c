/* **************************************************************************
 * vis - become visible
 */
nomask int
vis()
{
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!(tp->query_prop(OBJ_INVIS)))
    {
	tp->catch_tell("You are not invisible.\n");
	return 1;
    }
    tp->change_int_prop(OBJ_INVIS, -1);
    tp->catch_tell("You are now visible.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCNAME(tp), " " + tp->query_teleport_in() + "\n" }),
		 MSG_SEE, tp);
    return 1;
}
