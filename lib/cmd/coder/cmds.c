/* **************************************************************************
 * cmds - List all commands available for a player
 */
nomask int
cmds(string soul)
{
    int    i, sz;
    string *old_list, *list, *cmds;
    object ob, soulob;

    CHECK_LEVEL(_BUILDER);

    ob = this_interactive();
    
    old_list = ob->query_cmdsoul_list();
    old_list += ({ CODER_SOUL });
    
    if (soul)
	cmds = explode(soul, " ");

    for (i=0, sz=sizeof(old_list), list=allocate(sz); i<sz; i++)
    {
	if (!(soulob = find_object(old_list[i])) &&
	    catch(soulob = compile_object(old_list[i])))
	    continue;
	if (!soul || IN_ARRAY(soulob->get_soul_id(), cmds))
	    list[i] = old_list[i];
    }
    list -= ({ 0 });
    if (!(sz = sizeof(list)))
    {
	ob->catch_tell("No such soul.\n");
	return -1;
    }

    soul = "";
    for (i=0; i<sz; i++)
    {
	soul += "----- " + capitalize(list[i]->get_soul_id()) + ":\n";
	cmds = map_indices(list[i]->query_cmdlist());
	if (sizeof(cmds) > 0)
	    soul += BS(implode(cmds, ", "), 77, 0) + "\n";
    }
    smore(soul, ob);
    return 1;
}
