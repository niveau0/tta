/*
 * Function name: cancel
 * Description:   cancel a spell or cancel following actions
 * Arguments:     str - what to cancel (no argument cancels actions)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
cancel(string str, object tp)
{
    int    i, sz;
    string spell, on;
    object *list, *spells;

    if (!str)
    {
	/* cancel cmds is done in user object */
	tp->catch_tell("Ok.\n");
	return 1;
    }
    if (sscanf(str, "%s on %s", spell, on) != 2)
    {
	tp->catch_tell("Cancel which spell on whom?\n");
	return -1;
    }
    list = I(tp, 4) + tp->visible(I(E(tp), 0));
    list = parse(on, list, tp);
    if (!check_size(list, "No '" + on + "' to see.\n", tp))
	return -1;
    spells = list[0]->query_spells();
    spell  = SPELL_DIR + "/%*s/%*s/%*s/" + spell;
    for (i=0, sz=sizeof(spells); i<sz; i++)
	if (sscanf(MASTER_OB(spells[i]), spell) == 3)
	{
	    if (spells[i]->query_caster() == tp)
	    {
		spells[i]->remove_spell(list[0]);
		tp->catch_tell("Done.\n");
		return 1;
	    }
	}    
    tp->catch_tell("You sense no such spell there.\n");
    return 1;
}
