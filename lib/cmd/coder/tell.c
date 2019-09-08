/* **************************************************************************
 * tell - tell something to someone
 */
nomask int
tell(string str)
{
    string who, msg, pron, name;
    object ob, tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%s %s", who, msg) != 2)
    {
	tp->catch_tell("Tell whom what?\n");
	return -1;
    }
    ob = find_player(lower_case(who));
    if (!ob)
    {
	tp->catch_tell("No player with that name.\n");
	return -1;
    }
    ob->add_prop(CODER_REPLY, (name = tp->query_name()));
    if (!(name = ob->query_met(name))) /* avoid 'someone' in darkness */
	name = LANG_ADDART(tp->query_nonmet_name());
    else
	name = capitalize(name);
    ob->catch_tell(BS("A spectral image of " + name +
		      " appears in your mind, telling you:", 76, 0) +
		   "\n\n" + BS(msg, 76, 1) + "\n\nand the image disappears "+
		   "as quickly as it appeared.\n");
    tp->catch_tell("You tell " + capitalize(who) + ": " + msg +"\n");
    return 1;
}
