/* **************************************************************************
 * listboni, show all boni/penalties of a player
 */
nomask int
listboni(string str)
{
    object  tp, ob, *obj;
    
    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (!str)
	ob = tp;
    else if (sizeof(obj = parse(str, I(E(tp), 2), 0)))
	ob = obj[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such living.\n");
	return -1;
    }
    if (!living(ob))
    {
	tp->catch_tell("That's no living object.\n");
	return -1;
    }
    str = sprintf("\n_______________________Boni/Penalties_for_" +
		  "%_/-20s___________\n"+
		  " OB extra bonus__%_/5d   DB extra bonus__%_/5d\n"+
		  " Stunned_________%_/5s   Action Modifier_%_/5d  "+
		  " Elemental Def.__%_/5d\n Resist Men______%_/5d  "+
		  " Resist Ess______%_/5d   Resist Cha______%_/5d\n"+
		  " Resist Poison___%_/5d   Resist Disease__%_/5d  "+
		  " Resist Heat_____%_/5d\n Resist Cold_____%_/5d  "+
		  " Resist Impact___%_/5d   Resist Electr.__%_/5d\n",
		  capitalize(ob->query_name()),
		  ob->cb_query_bonus(CB_OB_BONUS),
		  ob->cb_query_bonus(CB_DB_BONUS),
		  (ob->cb_query_bonus(CB_STUNNED) > 0 ? "yes" : "no"),
		  ob->cb_query_bonus(CB_ACTION_MOD),
		  ob->cb_query_bonus(CB_ELEMENTAL_DEFENSE),
		  ob->cb_query_bonus(CB_RESIST_MEN),
		  ob->cb_query_bonus(CB_RESIST_ESS),
		  ob->cb_query_bonus(CB_RESIST_CHA),
		  ob->cb_query_bonus(CB_RESIST_POISON),
		  ob->cb_query_bonus(CB_RESIST_DISEASE),
		  ob->cb_query_bonus(CB_RESIST_HEAT),
		  ob->cb_query_bonus(CB_RESIST_COLD),
		  ob->cb_query_bonus(CB_RESIST_IMPACT),
		  ob->cb_query_bonus(CB_RESIST_ELECTRICITY));
    tp->catch_tell(str);
    return 1;
}
