/*
 * Function name: help
 * Description:   show help pages
 * Arguments:     arg - optional help page name
 * Returns:       1 if success, -1 else
 */
static nomask int
help(string arg)
{
    tp->catch_tell("Use 'edit' to enter the edit menu,\n" +
		   "    'view' to enter the view menu,\n" +
		   "    'jump' to jump around in the edit room\n" +
		   "    'save' to save your changes\n" +
		   "    'done' to leave the creator\n" +
		   "    'return' to jump back to edit room if you left it\n" +
		   "    'docu long|medium|short'\n" +
		   "           to toggle the documentation level\n");
    return 1;
}

static nomask string
documentation(string arg)
{
    int choice;
    
    if (doculevel)
	return "";
    
    sscanf(arg, "%d", choice);
    switch (choice)
    {
      case CLIMATE_CHOICE:
	  return HELP_CLIMATE;
      case COORDINATES_CHOICE:
	  return HELP_COORDINATES;
      case EXIT_CHOICE:
	  return HELP_EXIT;
      case ITEM_CHOICE:
	  return HELP_ITEM;
      case HEIGHT_CHOICE:
	  return HELP_HEIGHT;
      case LIGHT_CHOICE:
	  return HELP_LIGHT;
      case LONG_CHOICE:
	  return HELP_LONG;
      case OFFER_CHOICE:
	  return HELP_OFFER;
      case PARENT_CHOICE:
	  return HELP_PARENT;
      case PATH_CHOICE:
	  return HELP_PATH;
      case SHORT_CHOICE:
	  return HELP_SHORT;
      case SIZE_CHOICE:
	  return HELP_SIZE;
      case TYPE_CHOICE:
	  return HELP_TYPE;
      default:
	  return "";
    }
}

/*
 * Function name: docu
 * Description:   toggles the dokumentation level
 * Arguments:     arg - cmd argument
 * Returns:       1 if success, -1 else
 */
static nomask int
docu(string arg)
{
    switch(arg)
    {
      case "long": 
	  tp->catch_tell("Switched to long documentation.\n");
	  doculevel = 0;
	  return 1;
      case "medium":
	  tp->catch_tell("Switched to medium documentation.\n");
	  doculevel = 1;
	  return 1;
      case "short":
	  tp->catch_tell("Switched to short documentation.\n");
	  doculevel = 2;
	  return 1;
      default:
	  tp->catch_tell("Usage: docu long|medium|short\n");
	  return -1;
    }
}
