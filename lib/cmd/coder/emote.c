/* **************************************************************************
 * emote, : - emote something
 */
nomask int
emote(string str)
{
    object tp;
	
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: emote <emotiontext>\n" +
		       "       Note that your name is always added " +
		       "before the text.\n");
	return -1;
    }
    tp->catch_tell("You emote: " + capitalize(tp->query_name()) +
		   " " + str + "\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " " + str + "\n" }), MSG_SEE, tp);
    return 1;
}
