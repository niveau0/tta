/*********************************************************************
 * create_base.c
 * Base functions for the room creator
 */

static nomask int save_this();
static nomask int docu(string arg);

static int     doculevel, changeflag;
static object  tp;
static mapping editors;

private void
init_base(object player, string str)
{
    object other_editor;
    
    if (!str || object_name(previous_object()) != CODER_SOUL)
    {
	destruct_object();
	return;
    }
    tp = player;
    if (tp->query_prop(CURRENT_EDITOR))
    {
	tp->catch_tell("You are already in edit mode.\n");
	destruct_object();
        return;
    }
    room_file = FPATH(tp->query_path() + "/", str);
    if (other_editor = MASTER->query_editors()[room_file])
    {
    	tp->catch_tell("This room is currently edited by "+
		       capitalize(other_editor->query_name())+
		       ". Edit-mode not entered. \n");
	destruct_object();
	return;
    }
    if (room_file && !_ACCESS_WRITE(room_file, tp, 0))
    {
	tp->catch_tell("No write-access granted. Edit-mode aborted.\n");
	destruct_object();
	return;
    }
    if (file_size(room_file + ".o") > 0)
    {
	load();
	tp->catch_tell("Already exists, loading file to edit...\n");
    }
    else
    {
	tp->catch_tell("No such file '" + room_file +
		       ".o'. Creating new room.\n");
	room_coords = E(tp)->query_coordinates()[..];
    }
    tp->add_prop(STORED_ENV, E(tp));
    tp->move_living(this_object(), room_coords, "", MOVE_TELEPORT);
    tp->catch_tell("\nEntered room for edit. Use '?' for help.\n");
    docu("medium");
    tp->define_primary("catch_cmd", this_object());
    tp->add_prop(CURRENT_EDITOR, this_object());
    MASTER->add_editor(tp);
}

nomask void
add_editor(object tp)
{
    if (!editors)
	editors = ([ ]);
    editors[previous_object()->query_room_file()] = tp;
}

nomask void
remove_editor(object tp)
{
    if (!editors)
	return;
    editors[previous_object()->query_room_file()] = 0;
}

nomask mapping
query_editors()
{
    if (!editors)
	editors = ([ ]);
    return editors;
}

nomask object
query_editor()
{
    return tp;
}

/* never save a player in this room */
nomask int
query_no_save()
{
    return 1;
}

private string
view_result(string str)
{
    int    i, j, sz;
    string exit_status, out;
    mixed  ind, tmp, store, subexits;
#ifdef SHOP
    mixed shopoffer;
#endif
    
    switch (str)
    {
      case "climate":
	  if ((i = member_array(climate, CLIMATES)) == -1)
	      str = "none";
	  else
	      str = CLIMATES[i-1];
	  return "Current climate: " + str + ".\n";
      case "coord":
	  if (!room_coords)
	  {
	      return "No coordinates set.\n";
	  }
	  return "Current base coordinates (south west corner) " +
	      "[inch]: " + room_coords[0] + "x " + 
	      room_coords[1] + "y " + room_coords[2] + "z\n";
      case "exit":
	  if (!map_sizeof(room_exits))
	      return "Currently no exits.\n";
	  ind = map_indices(room_exits);
	  str = "Room exits:\n";
	  for (i=sizeof(ind); i--;)
	  {
	      tmp = room_exits[ind[i]];
	      switch (tmp[EXIT_DIR])
	      {
		case 'd': str += "down"; break;
		case 'u': str += "up"; break;
		case 'e': str += "east"; break;
		case 'w': str += "west"; break;
		case 'n': str += "north"; break;
		case 's': str += "south"; break;
	      }
	      str += ":\n";
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
	      str += sprintf("-- %-70s\n   Offset: %10d %10d   " +
			     "Width: %10d %10d  %s\n",
			     ind[i], tmp[EXIT_OFFX], tmp[EXIT_OFFY],
			     tmp[EXIT_WIDTH], tmp[EXIT_HEIGHT],
			     exit_status);
	  }
	  return str;
      case "item":
	  str = "Current pseudo items:\n";
	  tmp = query_item(-1);
	  ind = map_indices(tmp);
	  for (i=0, sz=sizeof(ind); i<sz; i++)
	  {
	      store = tmp[ind[i]];
	      str += "ID.: " + ind[i];
	      str += sprintf("\n%-78s\n%-78s",
			     "Sing.: " + implode(store[0], ", "),
			     "Plur.: " + implode(store[1], ", "));
	      if (sizeof(store) > 3)
		  str += sprintf("\nCoordinates: %dx, %dy, %dz",
				 store[3][0], store[3][1], store[3][2]);
	      str += "\n" + store[2] + "\n\n";
	  }
	  return str;
      case "height":
	  return "Room height [inch]: " + height +"\n";
      case "light":
	  return "Room light: " + light + "\n";
      case "long":
	  return "Long description:\n" + room_long;
      case "offer":
#ifndef SHOP
	  return "No offer, this is no shop.\n";
#else
	  shopoffer = query_offer();
	  ind = map_indices(shopoffer);
	  str = "Current offer:\nFile                         Amount | " +
		"Maximum | Prod.Time\n";
	  for(i=sizeof(ind); i--;)
	  {
	      tmp = shopoffer[ind[i]];
	      str += sprintf("%-30s %4d | %7d | %9d\n",
			     ind[i], tmp[0], tmp[2], tmp[3]);
	  }
	  return str;
#endif
      case "parent":
	  return "Room parent: " + parent_room +"\n";
      case "path":
	  return "Room path: " + room_file +"\n";
      case "short":
	  return "Short description:\n" + room_short + ".\n";
      case "size":
	  return "Room size [inch]: " + xsize + "x " + ysize + "y\n";
      case "type":
	  if (!type)
	      return "No type set.\n";
	  tmp = ROOM_TYPES;
	  str = "";
	  for (i=sizeof(tmp); i--;)
	  {
	      if (type & tmp[i--])
		  str += tmp[i] + "\n";
	  }
	  return "Currently set types:\n" + sprintf("% | *79s", str);
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
static int
view(string str)
{
    if (!str)
    {
	tp->catch_tell("Usage: view <attribute>\n");
	return -1;
    }
    if (str == "all")
    {
	smore(view_result("height") +
	      view_result("size") +
	      view_result("light") + 
	      view_result("parent") +
	      view_result("path") +
	      view_result("coord") +
	      view_result("climate") +
	      view_result("type") +
	      view_result("exit") +
	      view_result("item") +
	      view_result("short") +
	      view_result("long") +
	      view_result("offer"), tp);
	return 1;
    }
    smore(view_result(str), tp);
    return 1;
}

static nomask string
get_documentation(string arg)
{
    if (doculevel)
	return "";
    switch (arg)
    {
      case "climate":
	  return HELP_CLIMATE;
      case "coord":
	  return HELP_COORDINATES;
      case "exit":
	  return HELP_EXIT;
      case "item":
	  return HELP_ITEM;
      case "height":
	  return HELP_HEIGHT;
      case "light":
	  return HELP_LIGHT;
      case "long":
	  return HELP_LONG;
      case "offer":
	  return HELP_OFFER;
      case "parent":
	  return HELP_PARENT;
      case "path":
	  return HELP_PATH;
      case "short":
	  return HELP_SHORT;
      case "size":
	  return HELP_SIZE;
      case "type":
	  return HELP_TYPE;
      default:
	  return "";
    }
}

static nomask void
item_names(string str, int id)
{
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    if (str == "remove")
    {
	remove_item(id);
	tp->catch_tell("Tried to remove item " + id + ".\n");
	return;
    }
    tp->catch_tell("Item plural names [string,string,...]: ", 1);
    input_to("item_pnames", 0, id, explode(str, ","));
}

static nomask void
item_pnames(string str, int id, string *n)
{
    int i;
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    /* removing leading spaces from names */
    for ( i = sizeof(n)-1; i--;)
	while (n[i][0] == ' ')
	    n[i] = n[i][1..strlen(n[i])-1];
    
    tp->catch_tell("Item description [string]: ", 1);
    input_to("item_desc", 0, id, n, explode(str, ","));
}

static nomask void
item_desc(string str, int id, string *n, string *pn)
{
    int i;
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    /* removing leading spaces from plural names */
    for (i = sizeof(pn)-1; i--;)
	while (pn[i][0] == ' ')
	    pn[i] = pn[i][1..strlen(pn[i])-1];

    tp->catch_tell("Item coordinates or no entry [int,int,int]: ", 1);
    input_to("item_coord", 0, id, n, pn, str);
}

static nomask void
item_coord(string str, int id, string *n, string *pn, string desc)
{
    int x, y, z;
    
    if (!str || !strlen(str) || sscanf(str, "%d,%d,%d", x, y, z) != 3)
    {
	tp->catch_tell("Ok, assuming no coordinates.\n");
	add_item(n, pn, (desc[0] == '@' ? desc : BS(desc, 77, 0)), id);
    }
    else
	add_item(n, pn, (desc[0] == '@' ? str : BS(desc, 77, 0)), id,
		 ({ x, y, z }));
    tp->catch_tell("Item values set.\n");
}

/* This function will calculate the minimum offsets and the maximum sizes
 * for an exit to the given room.
 */
static nomask void
exit_full(int dir, string path, int *eco, int *esz)
{
    int off1, off2, size1max, size2max;
    
    switch (dir)
    {
      case 'n':
      case 's':
	  if ((off1 = eco[0] - room_coords[0]) < 0)
	      off1 = 0;
	  if ((off2 = eco[2] - room_coords[2]) < 0)
	      off2 = 0;
	  if ((size1max =
	       eco[0] + esz[0] - room_coords[0] - off1) > (xsize - off1))
	      size1max = xsize - off1;
	  if ((size2max =
	       eco[2] + esz[2] - room_coords[2] - off2) > (height - off2))
	      size2max = height - off2;
	  break;
      case 'u':
      case 'd':
	  if ((off1 = eco[0] - room_coords[0]) < 0)
	      off1 = 0;
	  if ((off2 = eco[1] - room_coords[1]) < 0)
	      off2 = 0;
	  if ((size1max =
	       eco[0] + esz[0] - room_coords[0] - off1) > (xsize - off1))
	      size1max = xsize - off1;
	  if ((size2max =
	       eco[1] + esz[1] - room_coords[1] - off2) > (ysize - off2))
	      size2max = ysize - off2;
	  break;
      case 'e':
      case 'w':
	  if ((off1 = eco[1] - room_coords[1]) < 0)
	      off1 = 0;
	  if ((off2 = eco[2] - room_coords[2]) < 0)
	      off2 = 0;
	  if ((size1max =
	       eco[1] + esz[1] - room_coords[1] - off1) > (ysize - off1))
	      size1max = ysize - off1;
	  if ((size2max =
	       eco[2] + esz[2] - room_coords[2] - off2) > (height - off2))
	      size2max = height - off2;
	  break;
    }
    room_exits[path] = ({ dir, off1, off2, size1max, size2max, 0, 0 });
    tp->catch_tell("Chosen minimal offsets: "+off1+" and "+off2+"\n"+
		   "Chosen maximal sizes:   "+size1max+" and "+size2max+"\n"+
		   "New exit set.\n");
}

static nomask void
exit_path(string path, int dir)
{
    int    *eco, *esz, off1min, off1max, off2min, off2max;
    string filepath, option;
    object ob;

    sscanf(path, "%s,%s", path, option);
    if (!path)
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    filepath = FPATH(room_file + "/..", path);
    if (!(ob = _ROOMD->load_room(filepath)))
    {
	tp->catch_tell("The exitroom does not exist.\n");
	return;
    }
    if (!(eco = ob->query_coordinates()))
    {
	tp->catch_tell("The exitroom has no coordinates.\n");
	return;
    }
    if (!sizeof(esz = ob->query_room_size()))
    {
	tp->catch_tell("The exitroom has no size.\n");
	return;
    }
    if (((eco[0] == room_coords[0] + xsize ||
	  eco[0] + esz[0] == room_coords[0]) &&
	 (eco[1] > room_coords[1] + ysize ||
	  eco[1] + esz[1] < room_coords[1])) ||
	((eco[1] == room_coords[1] + ysize ||
	  eco[1] + esz[1] == room_coords[1]) &&
	 (eco[0] > room_coords[0] + xsize ||
	  eco[0] + esz[0] < room_coords[0])))
    {
	tp->catch_tell("The exitroom's coordinates do not fit.\n");
	return;
    }
    if (option == "full")
    {
	exit_full(dir, path, eco, esz);
	return;
    }
    
    switch (dir)
    {
      case 'n':
      case 's':
	  if ((off1min = eco[0] - room_coords[0]) < 0)
	      off1min = 0;
	  if ((off1max = eco[0] + esz[0] - room_coords[0]) > xsize)
	      off1max = xsize;
	  if ((off2min = eco[2] - room_coords[2]) < 0)
	      off2min = 0;
	  if ((off2max = eco[2] + esz[2] - room_coords[2]) > height)
	      off2max = height;
	  tp->catch_tell("Range x-Offset: "+off1min+" - "+off1max+"\n"+
			 "Range z-Offset: "+off2min+" - "+off2max+"\n"+
			 "Enter x and z coordinate offset [inch,inch]: ", 1);
	  break;
      case 'u':
      case 'd':
	  if ((off1min = eco[0] - room_coords[0]) < 0)
	      off1min = 0;
	  if ((off1max = eco[0] + esz[0] - room_coords[0]) > xsize)
	      off1max = xsize;
	  if ((off2min = eco[1] - room_coords[1]) < 0)
	      off2min = 0;
	  if ((off2max = eco[1] + esz[1] - room_coords[1]) > ysize)
	      off2max = ysize;
	  tp->catch_tell("Range x-Offset: "+off1min+" - "+off1max+"\n"+
			 "Range y-Offset: "+off2min+" - "+off2max+"\n"+
			 "Enter x and y coordinate offset [inch,inch]: ", 1);
	  break;
      case 'e':
      case 'w':
	  if ((off1min = eco[1] - room_coords[1]) < 0)
	      off1min = 0;
	  if ((off1max = eco[1] + esz[1] - room_coords[1]) > ysize)
	      off1max = ysize;
	  if ((off2min = eco[2] - room_coords[2]) < 0)
	      off2min = 0;
	  if ((off2max = eco[2] + esz[2] - room_coords[2]) > height)
	      off2max = height;
	  tp->catch_tell("Range y-Offset: "+off1min+" - "+off1max+"\n"+
			 "Range z-Offset: "+off2min+" - "+off2max+"\n"+
			 "Enter y and z coordinate offset [inch,inch]: ", 1);
	  break;
    }
    input_to("exit_offsets", 0, dir, path);
}

static nomask void
exit_offsets(string str, int dir, string path)
{
    int *eco, *esz, off1, off2, size1max, size2max;
    string filepath;
    object ob;
    
    if (!str || sscanf(str, "%d,%d", off1, off2) != 2)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    tp->catch_tell("\nEnter 'remove' to remove the exit or 'status'"+
		   "to edit its status.\n",1);
    filepath = FPATH(room_file + "/..", path);
    ob = _ROOMD->load_room(filepath);
    eco = ob->query_coordinates();
    esz = ob->query_room_size();
    switch (dir)
    {
      case 'n':
      case 's':
	  if ((size1max =
	       eco[0] + esz[0] - room_coords[0] - off1) > (xsize - off1))
	      size1max = xsize - off1;
	  if ((size2max =
	       eco[2] + esz[2] - room_coords[2] - off2) > (height - off2))
	      size2max = height - off2;
	  tp->catch_tell("Maximum x-width : "+size1max+"\n"+
			 "Maximum z-height: "+size2max+"\n", 1);
	  break;
      case 'u':
      case 'd':
	  if ((size1max =
	       eco[0] + esz[0] - room_coords[0] - off1) > (xsize - off1))
	      size1max = xsize - off1;
	  if ((size2max =
	       eco[1] + esz[1] - room_coords[1] - off2) > (ysize - off2))
	      size2max = ysize - off2;
	  tp->catch_tell("Maximum x-width : "+size1max+"\n"+
			 "Maximum y-height: "+size2max+"\n", 1);
	  break;
      case 'e':
      case 'w':
	  if ((size1max =
	       eco[1] + esz[1] - room_coords[1] - off1) > (ysize - off1))
	      size1max = ysize - off1;
	  if ((size2max =
	       eco[2] + esz[2] - room_coords[2] - off2) > (height - off2))
	      size2max = height - off2;
	  tp->catch_tell("Maximum y-width : "+size1max+"\n"+
			 "Maximum z-height: "+size2max+"\n", 1);
	  break;
    }
    tp->catch_tell("Enter width and height (or 'outside') "+
		   "[inch,inch]: ", 1);
    input_to("exit_size", 0, dir, path, off1, off2);
}


static nomask void
exit_size(string str, int dir, string path, int off1, int off2)
{
    int w, h;
    string str_h;
    
    if (str == "remove")
    {
	if (!room_exits[path])
	{
	    tp->catch_tell("No such exit, cancelled.\n");
	    return;
	}
	room_exits[path] = 0;
	tp->catch_tell("Exit removed.\n");
	return;
    }
    if (str == "status")
    {
	tp->catch_tell("Enter the new exit status [integer]:", 1);
	input_to("edit_exit_status", 0, path, dir, off1, off2);
	return;
    }
    if (!str || sscanf(str, "%d,%d", w, h) != 2)
    {
	sscanf(str, "%d,%s", w, str_h);
	if(str_h != "outside")
	{
	    tp->catch_tell("Wrong syntax, cancelled.\n");
	    return;
	}
	else
	    h = DEFAULT_ROOM_HEIGHT;
    }
    room_exits[path] = ({ dir, off1, off2, w, h, 0, 0 });
    tp->catch_tell("New exit set.\n");
}

static nomask void
edit_exit_status(string str, string path, int dir, int off1, int off2)
{
    tp->catch_tell("New status set to "+str+".\n");
    room_exits[path][EXIT_STATUS] = (int)str;
}

/****************************************************************************
 * This function will create and save exits in all rooms which are
 * connected to the currently edited room. (Existing exits have to
 * be correct.)
 */
void
spread_exits()
{
    int i_exit, dir, off1, off2, offdir1, offdir2, *eco, *esz,width, height,
	i_array;
    string *ind, filepath, this_room, dummy_string, *relpath1, *relpath2,
	dir_string;
    mapping editors;
    object ob;

    editors = MASTER->query_editors();
    ind = map_indices(room_exits);

    for (i_exit = sizeof(ind)-1; i_exit >= 0; i_exit--)
    {
	filepath = FPATH(room_file + "/..", ind[i_exit]);
	if (!editors[filepath]) // If room is not currently edited.
	{
	    ob = _ROOMD->load_room(filepath);
	    eco = ob->query_coordinates();
	    esz = ob->query_room_size();
	    // Computing the data for the corresponding exit
	    switch ( room_exits[ind[i_exit]][EXIT_DIR] )
	    {
	      case 'd':
		  dir = 'u';
		  dir_string = "up";
		  offdir1 = 0;
		  offdir2 = 1;
		  break;
	      case 'u':
		  dir = 'd';
		  dir_string = "down";
		  offdir1 = 0;
		  offdir2 = 1;
		  break;		  
	      case 'n':
		  dir = 's';
		  dir_string = "south";
		  offdir1 = 0;
		  offdir2 = 2;
		  break;
	      case 's':		  
		  dir = 'n';		  
		  dir_string = "north";
		  offdir1 = 0;
		  offdir2 = 2;
		  break;		  
	      case 'e':		  
		  dir = 'w';
		  dir_string = "west";
		  offdir1 = 1;
		  offdir2 = 2;
		  break;
	      case 'w':
		  dir = 'e';
		  dir_string = "east";
		  offdir1 = 1;
		  offdir2 = 2;
		  break;
	      default:
		  return;
	    }
	    off1 = room_coords[offdir1] + room_exits[ind[i_exit]][EXIT_OFFX]
		   - eco[offdir1];
	    off2 = room_coords[offdir2] + room_exits[ind[i_exit]][EXIT_OFFY]
		   - eco[offdir2];
	    width = room_exits[ind[i_exit]][EXIT_WIDTH];
	    height = room_exits[ind[i_exit]][EXIT_HEIGHT];

	    // Looking for the relative filepath of the new exit
//	      filepath = FPATH(ob->query_room_file()+"/..", "");
//	      filepath = filepath[0..strlen(filepath)-2];
//	      this_room = "";
//	      while( room_file[0..strlen(filepath)-1]!=filepath)
//	      {
//		  this_room = this_room + "/..";
//		  filepath = FPATH(filepath + "/..", "");
//		  filepath = filepath[0..strlen(filepath)-2];
//	      }
//	      this_room = this_room+
//			  room_file[strlen(filepath)..strlen(room_file)-1];
	    relpath1 = explode(room_file, "/");
	    relpath2 = explode(filepath, "/");
	    i_array = 0;
	    while (relpath1[i_array] == relpath2[i_array])
	    {
		relpath1[i_array] = 0;
		relpath2[i_array] = 0;
		i_array++;
	    }
	    relpath1 -= ({ 0 });
	    dummy_string = implode(relpath1, "/");
	    relpath2 -= ({ 0 });
	    this_room = "";
	    for (i_array = sizeof(relpath2)-1; i_array > 0; i_array--)
	    {
		this_room = this_room + "../";
	    }
	    this_room = this_room + dummy_string;

	    if (ob->query_exits()[this_room])
		// Does this exit already exists in other room?
	    {
		tp->catch_tell("Exit already exists in "+ind[i_exit]+
			       ", values altered to:\n"+
			       "Dir:"+dir_string+" Offset: "+off1+" "+off2+
			       " Width:"+width+" Height:"+height+"\n\n");
	    }
	    else
	    {
		tp->catch_tell("Created new exit in "+ind[i_exit]+"\n"+
			       "Dir:"+dir_string+" Offset: "+off1+" "+off2+
			       " Width:"+width+" Height:"+height+"\n\n");
	    }
	    ob->add_exit( this_room,
			  ({ dir, off1, off2, width, height, 0, 0 }) );
	}
	else
	{
	    tp->catch_tell("Room '"+ind[i_exit]+
		"' is currently edited. Nothing changed there.\n");
	}
    }
}

/****************************************************************************
 * Functions called by edit command
 */
static nomask void
do_climate(string str)
{
    int i;
    
    if ((i = member_array(str, CLIMATES)) == -1)
	tp->catch_tell("No such climate, cancelled.\n");
    else
    {
	climate = CLIMATES[i+1];
	tp->catch_tell("New climate set.\n");
    }
}

static nomask void
do_coord(string str)
{
    int x, y, z;
    
    if (!str || sscanf(str, "%d,%d,%d", x, y, z) != 3)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    room_coords = ({ x, y, z });
    tp->set_coordinates(room_coords);
    tp->catch_tell("New coordinates set. You have been moved.\n");
}

static nomask void
do_exit(string str)
{
    int dir;
    
    if (!str)
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    if (str == "spread")
    {
	spread_exits();
	return;
    }
    switch (str)
    {
      case "d":
      case "down":
	  dir = 'd';
	  break;
      case "u":
      case "up":
	  dir = 'u';
	  break;
      case "n":
      case "north":
	  dir = 'n';
	  break;
      case "s":
      case "south":
	  dir = 's';
	  break;
      case "e":
      case "east":
	  dir = 'e';
	  break;
      case "w":
      case "west":
	  dir = 'w';
	  break;
      default:
	  tp->catch_tell("Invalid direction: '" + str + "'.\n");
	  return;
    }
    tp->catch_tell("Exitroom [,'full'] [pathname[,string]]: ", 1);
    input_to("exit_path", 0, dir);
}

static nomask void
do_height(string str)
{
    int h;

    if (str == "default")
	h = DEFAULT_ROOM_HEIGHT;
    else if (!str || !sscanf(str, "%d", h) || h < 1)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    height = h; 
    tp->catch_tell("New height set.\n");
}

static nomask void
do_item(string str)
{
    int id;
    mapping tmp;
    
    if (!str || (!sscanf(str, "%d", id) && str != "new"))
    {
	if(str != "new" && id < 1)
	{
	tp->catch_tell("Wrong value, cancelled.\n");
	return;
	}
    }
    if (str == "new")
    {
	tmp = query_item(-1);
	id = sizeof(map_indices(tmp))+1;
	while (tmp[id])
	    id++;
	tp->catch_tell("Creating new item #"+id+"\n",1);
    }
    
    tp->catch_tell("Item names [string,string,...] or " +
		   "'remove': ", 1);
    input_to("item_names", 0, id);
}

static nomask void
do_light(string str)
{
    int l;
    
    if (!str || !sscanf(str, "%d", l))
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    light = l;
    tp->catch_tell("New light set.\n");
}

nomask void
do_long(string str)
{
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    room_long = str;
    tp->catch_tell("New long description set.\n");
}

static nomask void
do_offer(string str)
{
#ifdef SHOP
    int    count, max, t;
    string file;
    
    if (!str || sscanf(str, "%s,%d,%d,%d", file, count, max, t) != 4)
    {
	if (str && sscanf(str, "%s,remove", file))
	{
	    remove_offer(file);
	    tp->catch_tell("Removed " + file + " from shop offer.\n");
	}
	else
	    tp->catch_tell("Wrong syntax, cancelled\n");
	return;
    }
    add_offer(file, count, max, t);
    tp->catch_tell("New offer added.\n");
#endif
}

static nomask void
do_parent(string str)
{
    if (!str || sscanf(str, "/d/%*s/%*s") != 2)
    {
	tp->catch_tell("Invalid parentpath. Allowed is: " +
		       "/d/<Domain>/...\n");
	return;
    }
    parent_room = str;
    tp->catch_tell("New parent set.\n");
}

static nomask void
do_path(string str)
{
    if (!str || (sscanf(str, "/d/%*s/%*s") != 2 &&
		 sscanf(str, "/home/%*s/%*s") != 2))
    {
	tp->catch_tell("Invalid roompath. Allowed are: " +
		       "/d/<Domain>/... or /home/<yourname>/...\n");
	return;
    }
    room_file = str;
    tp->catch_tell("New path set.\n");
}

static nomask void
do_short(string str)
{
    room_short = BS(str, 78, 0);
    tp->catch_tell("New short description set.\n");
}

static nomask void
do_size(string str)
{
    int x, y;
    
    if (!str || sscanf(str, "%d,%d", x, y) != 2)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    xsize = x;
    ysize = y;
    tp->catch_tell("New size set.\n");
}

static nomask void
do_type(string str)
{
    int i;
    
    if ((i = member_array(str, ROOM_TYPES)) == -1)
	tp->catch_tell("No such type.\n");
    else
    {
	type ^= ROOM_TYPES[i+1];
	tp->catch_tell("New type set (toggled).\n");
    }
}
    
/*
 * Function name: edit
 * Description:   choose something to change
 * Arguments:     arg - what to change
 * Returns:       1 if successful, -1 else
 */
static nomask int
edit(string cmd)
{
    int    i;
    string arg;
    mixed  tmp;

    if (cmd)
	sscanf(cmd, "%s %s", cmd, arg);
    if (arg)
    {
	switch (cmd)
	{
	  case "climate":
	      do_climate(arg);
	      return 1;
	  case "coord":
	      do_coord(arg);
	      return 1;
	  case "exit":
	      do_exit(arg);
	      return 1;
	  case "height":
	      do_height(arg);
	      return 1;
	  case "item":
	      do_item(arg);
	      return 1;
	  case "light":
	      do_light(arg);
	      return 1;
	  case "offer":
#ifdef SHOP
	      do_offer(arg);
#else
	      tp->catch_tell("This is no shop.\n");
#endif
	      return 1;
	  case "path":
	      do_path(arg);
	      return 1;
	  case "short":
	      do_short(arg);
	      return 1;
	  case "size":
	      do_size(arg);
	      return 1;
	  case "type":
	      do_type(arg);
	      return 1;
	  default:
	      tp->catch_tell("No such edit command, try '?' for help.\n");
	      return -1;
	}
    }
    tp->catch_tell(get_documentation(cmd));
    if (doculevel <= 1)
	smore(view_result(cmd), tp);
    switch (cmd)
    {
      case "climate":
	  tmp = CLIMATES;
	  for (i=sizeof(tmp); i--;)
	      if (intp(tmp[i]))
		  tmp[i] = 0;
	  tmp -= ({ 0 });
	  tp->catch_tell(BS(implode(tmp, ", "), 77, 0) +
			 "\nNew climate [string]: ");
	  input_to("do_climate");
	  return 1;
      case "coord":
	  tp->catch_tell("New coordinates (x y z) [inch,inch,inch]: ");
	  input_to("do_coord");
	  return 1;
      case "exit":
	  tp->catch_tell("New exit direction [string|'spread']: ");
	  input_to("do_exit");
	  return 1;
      case "height":
	  tp->catch_tell("New height [inch] or 'default' (outside): ");
	  input_to("do_height");
	  return 1;
      case "item":
	  tp->catch_tell("Item number or 'new' [integer]: ");
	  input_to("do_item");
	  return 1;
      case "light":
	  tp->catch_tell("New light [integer]: ");
	  input_to("do_light");
	  return 1;
      case "long":
	  tp->catch_tell("Please edit the long description.\n");
	  EDIT("do_long", room_long);
	  return 1;
      case "offer":
#ifdef SHOP
	  tp->catch_tell("New offer [path,amount,max.amount," +
			 "production time] or [path,remove]:\n");
	  input_to("do_offer");
#else
	  tp->catch_tell("This is no shop.\n");
#endif
	  return 1;
      case "parent":
	  tp->catch_tell("New parent [pathname]: ");
	  input_to("do_parent");
	  return 1;
      case "path":
	  tp->catch_tell("New filepath [pathname]: ");
	  input_to("do_path");
	  return 1;
      case "short":
	  tp->catch_tell("New short description [string]: ");
	  input_to("do_short");
	  return 1;
      case "size":
	  tp->catch_tell("Room size (x y) [inch,inch]: ");
	  input_to("do_size");
	  return 1;
      case "type":
	  tmp = ROOM_TYPES;
	  for (i=sizeof(tmp); i--;)
	      if (intp(tmp[i]))
		  tmp[i] = 0;
	  tmp -= ({ 0 });
	  tp->catch_tell(BS(implode(tmp, ", "), 77, 0) +
			 "\nNew type [string]: ");
	  input_to("do_type");
	  return 1;
      default:
	  tp->catch_tell("No such edit command, try '?' for help.\n");
	  return -1;
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

/*
 * Function name: jump
 * Description:   jump around in the room
 * Arguments:     arg - the offsets to jump
 * Returns:       1 if success, -1 else
 */
static nomask int
jump(string arg)
{
    int x, y, z, *co;
    
    if (!arg || sscanf(arg, "%d %d %d", x, y, z) != 3)
    {
	tp->catch_tell("Usage: jump xoffset yoffset zoffset\n");
	return -1;
    }
    co = tp->query_coordinates()[..];
    co[0] += x;
    co[1] += y;
    co[2] += z;
    if (query_leaving_room(co) != this_object())
    {
	tp->catch_tell("You would leave the room with these offsets!\n");
	return -1;
    }
    tell_objects(LISTEN(this_object()) - ({ tp }),
		 ({ QCTNAME(tp), " jumps through the room.\n" }), MSG_SEE, tp);
    tp->catch_tell("Ok, you jumped.\n");
    tp->set_coordinates(co);  
    return 1;
}

/*
 * Function name: help
 * Description:   show help pages
 * Arguments:     arg - optional help page name
 * Returns:       1 if success, -1 else
 */
static nomask int
help(string arg)
{
    tp->catch_tell("Use 'edit' to enter the edit menu," +
		   "    'view' to enter the view menu," +
		   "    'jump' to jump around in the edit room\n" +
		   "    'save' to save your changes\n" +
		   "    'done' to leave the creator\n" +
		   "    'return' to jump back to edit room if you left it" +
		   "    'docu long|medium|short'\n" +
		   "           to toggle the documentation level\n" +
		   "", tp);
    return 1;
}

/*
 * Function name: help
 * Description:   show help pages
 * Arguments:     arg - optional help page name
 * Returns:       1 if success, -1 else
 */
static nomask int
edit(string arg)
{
    tp->catch_tell(sprintf("% *77s",
			   " 1) Coordinates\n" +
			   " 2) Height\n" +
			   " 3) Size\n" +
			   " 4) Light\n" +
			   " 5) Exits\n" +
			   " 6) Climate\n" +
			   " 7) Type\n" +
			   " 8) Items\n" +
			   " 9) ShortDesc\n" +
			   "10) LongDesc\n" +
			   "11) Offer\n" +
			   "12) Parent\n" +
			   "13) Path\n"));
    return 1;
}

private void
set_changeflag()
{
    changeflag |= CHANGED;
}

/*
 * Function name: catch_cmd 
 * Description:   catch every command while editing
 * Arguments:     cmd - command
 *                arg - command argument
 * Returns:       1 if successful, -1 if not, 0 if not valid
 */
nomask int
catch_cmd(string cmd, string arg)
{
    object r;
    
    if (!tp)
    {
	remove_object();
	return 0;
    }
    if (cmd == "done")
    {
	if (changeflag & CHANGED)
	{
	    changeflag ^= CHANGED;
	    call_out("set_changeflag", 10);
	    tp->catch_tell("There were changes made, the room is " +
			   "not saved.\nTo enforce exit, type 'done' again " +
			   "within 10 seconds.\n");
	    return -1;
	}
	tp->undefine_primary("catch_cmd", this_object());
	if (E(tp) == this_object())
	{
	    if (!(r = tp->query_prop(STORED_ENV)))
	    {
		tp->catch_tell("Failed to find your old location, please " +
			       "leave the edit room and\nuse 'done' again.\n");
		return -1;
	    }
	    tp->move_living(r, r->query_coordinates(), "", 1);
	}
	if (changeflag & SAVED)
	    tp->catch_tell("\n*** Enter " + room_file + " and use\n" +
			   "*** command 'uh' to update your changes\n");
	MASTER->remove_editor(tp);
	remove_object();
	return 1;
    }
    if (tp != this_player())
	return 0;
    switch (cmd)
    {
      case "?":
	  return help(arg);
      case "docu":
	  return docu(arg);
      case "edit":
	  changeflag |= CHANGED;
	  return edit(arg);
      case "jump":
	  return jump(arg);
      case "view":
	  return view(arg);
      case "return":
	  tp->move_living(this_object(), query_coordinates(), "", 1);
	  return 1;
      case "save":
      {
	  if (room_file && !_ACCESS_WRITE(room_file, tp, 0))
	      room_file = 0;
	  if (!room_file || (sscanf(room_file, "/d/%*s/%*s") != 2 &&
			     sscanf(room_file, "/home/%*s/%*s") != 2))
	  {
	      tp->catch_tell("Invalid roompath. Allowed are: " +
			     "/d/<Domain>/... or /home/<yourname>/...\n");
	      return -1;
	  }
	  changeflag -= changeflag & CHANGED;
	  changeflag |= SAVED;
	  compute_corner_height();
	  return save_this();
      }
      default:
	  return 0;
    }
}
