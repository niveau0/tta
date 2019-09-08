/* calculates the minimum offsets and the maximum size
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
static nomask void
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
