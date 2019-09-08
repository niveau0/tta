/* **************************************************************************
 * heal - obvious, heal something
 */
nomask int
heal(string str)
{
    int     i;
    object  tp;
    mapping map;
    mixed   ind;
    
    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    switch (str)
    {
      case "wounds":
      case "wound":
	  tp->add_hp(tp->query_max_hp());
	  tp->echo("Restored all hitpoints.\n");
	  return 1;
      case "blood":
      case "bloodloss":
	  map = tp->query_bloodwounds();
	  for (i=sizeof(ind=map_indices(map)); i--; )
	      tp->add_bloodloss(-map[ind[i]], ind[i]);
	  tp->echo("Removed all bleeding wounds.\n");
	  return 1;
      case "fractures":
	  map = tp->query_wounds();
	  for (i=sizeof(ind=map_indices(map)); i--; )
	      tp->set_wound(ind[i], 0);
	  tp->echo("Removed all fractures.\n");
	  return 1;
      case "effects":
	  break;
      default:
	  tp->catch_tell("Usage: heal wounds|bloodloss|fractures|effects\n");
	  return -1;
    }
    tp->catch_tell("Not implemented.\n");
    return 1;
}
