static nomask string
view_result(int choice)
{
    int    i, j, sz;
    string exit_status, out;
    mixed  ind, tmp, store, subexits;
#ifdef SHOP
    mixed shopoffer;
#endif
    
    switch (choice)
    {
      case CLIMATE_CHOICE:
	  if ((i = member_array(climate, CLIMATES)) == -1)
	      out = "none";
	  else
	      out = CLIMATES[i-1];
	  return "Current climate: " + out + ".\n";
      case COORDINATES_CHOICE:
	  if (!room_coords)
	      return "No coordinates set.\n";
	  return "Current base coordinates (south west corner) " +
	      "[inch]: " + room_coords[0] + "x " + 
	      room_coords[1] + "y " + room_coords[2] + "z\n";
      case EXIT_CHOICE:
	  if (!map_sizeof(room_exits))
	      return "Currently no exits.\n";
	  ind = map_indices(room_exits);
	  out = "Room exits:\n";
	  for (i=sizeof(ind); i--;)
	  {
	      tmp = room_exits[ind[i]];
	      switch (tmp[EXIT_DIR])
	      {
		case 'd': out += "down"; break;
		case 'u': out += "up"; break;
		case 'e': out += "east"; break;
		case 'w': out += "west"; break;
		case 'n': out += "north"; break;
		case 's': out += "south"; break;
	      }
	      out += ":\n";
	      switch (tmp[EXIT_STATUS])
	      {
		case EXIT_OPEN:
		    exit_status = "open";
		    break;
		case EXIT_INVIS:
		    exit_status = "opaque";
		    break;
		case EXIT_CLOSED:
		    exit_status = "closed";
		    break;
		default:
		    exit_status = "unknown";
	      }
	      exit_status = exit_status + "(" + tmp[EXIT_STATUS] + ")";
	      out += sprintf("-- %-70s\n   Offset: %10d %10d   " +
			     "Width: %10d %10d  %s\n",
			     ind[i], tmp[EXIT_OFFX], tmp[EXIT_OFFY],
			     tmp[EXIT_WIDTH], tmp[EXIT_HEIGHT],
			     exit_status);
	  }
	  return out;
      case ITEM_CHOICE:
	  out = "Current pseudo items:\n";
	  tmp = query_item(-1);
	  ind = map_indices(tmp);
	  for (i=0, sz=sizeof(ind); i<sz; i++)
	  {
	      store = tmp[ind[i]];
	      out += "ID.: " + ind[i];
	      out += sprintf("\n%-78s\n%-78s",
			     "Singular: " + implode(store[0], ", "),
			     "Plural:   " + implode(store[1], ", "));
	      if (sizeof(store) > 3)
		  out += sprintf("\nCoordinates: %dx, %dy, %dz",
				 store[3][0], store[3][1], store[3][2]);
	      out += "\n" + store[2] + "\n\n";
	  }
	  return out;
      case HEIGHT_CHOICE:
	  return "Room height [inch]: " + height +"\n";
      case LIGHT_CHOICE:
	  return "Room light: " + light + "\n";
      case LONG_CHOICE:
	  return "Long description:\n" + room_long;
      case OFFER_CHOICE:
#ifndef SHOP
	  return "No offer, this is no shop.\n";
#else
	  shopoffer = query_offer();
	  ind = map_indices(shopoffer);
	  out = "Current offer:\nFile                         Amount | " +
		"Maximum | Prod.Time\n";
	  for(i=sizeof(ind); i--;)
	  {
	      tmp = shopoffer[ind[i]];
	      out += sprintf("%-30s %4d | %7d | %9d\n",
			     ind[i], tmp[0], tmp[2], tmp[3]);
	  }
	  return out;
#endif
      case PARENT_CHOICE:
	  return "Room parent: " + parent_room +"\n";
      case PATH_CHOICE:
	  return "Room path: " + room_file +"\n";
      case SHORT_CHOICE:
	  return "Short description:\n" + room_short + ".\n";
      case SIZE_CHOICE:
	  return "Room size [inch]: " + xsize + "x " + ysize + "y\n";
      case TYPE_CHOICE:
	  if (!type)
	      return "No type set.\n";
	  tmp = ROOM_TYPES;
	  out = "";
	  for (i=sizeof(tmp); i--;)
	  {
	      if (type & tmp[i--])
		  out += tmp[i] + "\n";
	  }
	  return "Currently set types:\n" + sprintf("% | *79s", out);
      case BORDER_CHOICE:
	  return "Borderlevels:       north:"+edge_base[ROOM_EDGE_N]+"\n"+
	      "             west:"+edge_base[ROOM_EDGE_W]+"         east:"+
	      edge_base[ROOM_EDGE_E]+"\n"+
	      "                    south:"+edge_base[ROOM_EDGE_S]+"\n";
      default:
	  return "There is no such attribute to 'view'.\n";
    }
}

/*
 * Function name: view
 * Description:   view a room attribute
 * Arguments:     arg - which attribute
 * Returns:       1 if success, -1 else
 */
static nomask int
view(string arg)
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
			   "a) View all\n" +
			   "q) Exit menu\n") +
		   "Choice: ", 1);
    input_to("view_choice", 0);
    return 1;
}

/*
 * Function name: view_choice
 * Description:   choose something to look at
 * Arguments:     arg - what to view
 */
static nomask void
view_choice(string arg)
{
    int choice;
    
    if (arg == "q")
    {
	tp->echo("Left view menu.\n");
	return;
    }
    if (arg == "a")
    {
	smore(view_result(CLIMATE_CHOICE) +
	      view_result(COORDINATES_CHOICE) +
	      view_result(SIZE_CHOICE) +
	      view_result(HEIGHT_CHOICE) +
	      view_result(BORDER_CHOICE) +
	      view_result(EXIT_CHOICE) + 
	      view_result(ITEM_CHOICE) +
	      view_result(LIGHT_CHOICE) +
	      view_result(LONG_CHOICE) +
	      view_result(OFFER_CHOICE) +
	      view_result(PARENT_CHOICE) +
	      view_result(PATH_CHOICE) +
	      view_result(SHORT_CHOICE) +
	      view_result(TYPE_CHOICE), tp);
    }
    else if (!arg)
	tp->catch_tell("No such value to view.\n", 1);
    else
    {
	sscanf(arg, "%d", choice);
	smore(view_result(choice), tp);
    }
}
