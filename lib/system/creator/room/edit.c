/*
 * Function name: edit
 * Description:   show edit menu
 * Returns:       1 if success, -1 else
 */
static nomask int
edit()
{
    tp->catch_tell(sprintf("% *77s\n",
			   CLIMATE_CHOICE + ") Climate\n" +
			   COORDINATES_CHOICE + ") Coordinates\n" +
			   EXIT_CHOICE + ") Exits\n" +
			   HEIGHT_CHOICE + ") Height\n" +
			   ITEM_CHOICE + ") Items\n" +
			   LIGHT_CHOICE + ") Light\n" +
			   LONG_CHOICE + ") LongDesc\n" +
			   OFFER_CHOICE + ") Offer\n" +
			   PARENT_CHOICE + ") Parent\n" +
			   PATH_CHOICE + ") Path\n" +
			   SHORT_CHOICE + ") ShortDesc\n" +
			   SIZE_CHOICE + ") Size\n" +
			   TYPE_CHOICE + ") Type\n" +
			   BORDER_CHOICE + ") Borderlevel\n" +
			   " q) Exit menu\n") +
		   "Choice: ", 1);
    input_to("edit_choice", 0);
    return 1;
}

/*
 * Function name: edit_choice
 * Description:   choose something to change
 * Arguments:     arg - what to change
 */
static nomask void
edit_choice(string arg)
{
    int    i, cmd, choice;
    mixed  tmp;

    if (arg == "q")
    {
	tp->echo("Left edit menu.\n");
	return;
    }
    
    tp->catch_tell(documentation(arg), 1);
    if (arg)
	sscanf(arg, "%d", choice);
    if (doculevel <= 1)
	tp->catch_tell(view_result(choice), 1);

    switch (choice)
    {
      case CLIMATE_CHOICE:
	  tmp = CLIMATES;
	  for (i=sizeof(tmp); i--;)
	      if (intp(tmp[i]))
		  tmp[i] = 0;
	  tmp -= ({ 0 });
	  tp->catch_tell(BS(implode(tmp, ", "), 77, 0) +
			 "\nNew climate [string]: ", 1);
	  input_to("do_climate");
	  return;
      case COORDINATES_CHOICE:
	  tp->catch_tell("New coordinates (x y z) [inch,inch,inch]: ", 1);
	  input_to("do_coord");
	  return;
      case EXIT_CHOICE:
	  tp->catch_tell("New exit direction [string|'spread']: ", 1);
	  input_to("do_exit");
	  return;
      case HEIGHT_CHOICE:
	  tp->catch_tell("New height [inch] or 'default' (outside): ", 1);
	  input_to("do_height");
	  return;
      case ITEM_CHOICE:
	  tp->catch_tell("Item number or 'new' [integer]: ", 1);
	  input_to("do_item");
	  return;
      case LIGHT_CHOICE:
	  tp->catch_tell("New light [integer]: ", 1);
	  input_to("do_light");
	  return;
      case LONG_CHOICE:
	  tp->catch_tell("Please edit the long description.\n", 1);
	  EDIT("do_long", room_long);
	  return;
      case OFFER_CHOICE:
#ifdef SHOP
	  tp->catch_tell("New offer [path,amount,max.amount," +
			 "production time] or [path,remove]:\n", 1);
	  input_to("do_offer");
#else
	  tp->catch_tell("This is no shop.\n", 1);
#endif
	  return;
      case PARENT_CHOICE:
	  tp->catch_tell("New parent [pathname]: ", 1);
	  input_to("do_parent");
	  return;
      case PATH_CHOICE:
	  tp->catch_tell("New filepath [pathname]: ", 1);
	  input_to("do_path");
	  return;
      case SHORT_CHOICE:
	  tp->catch_tell("New short description [string]: ", 1);
	  input_to("do_short");
	  return;
      case SIZE_CHOICE:
	  tp->catch_tell("Room size (x y) [inch,inch]: ", 1);
	  input_to("do_size");
	  return;
      case BORDER_CHOICE:
	  tp->catch_tell("Level of borders (north east south west):", 1);
	  input_to("do_border");
	  return;
      case TYPE_CHOICE:
	  tmp = ROOM_TYPES;
	  for (i=sizeof(tmp); i--;)
	      if (intp(tmp[i]))
		  tmp[i] = 0;
	  tmp -= ({ 0 });
	  tp->catch_tell(BS(implode(tmp, ", "), 77, 0) +
			 "\nNew type [string]: ", 1);
	  input_to("do_type");
	  return;
      default:
	  tp->catch_tell("No such value to edit.\n", 1);
	  edit();
	  return;
    }
}
