static int    line;
static string finished_fun, *lines;
static object calling_ob, player;

nomask object
query_editor()
{
    return player;
}

static nomask void
display()
{
    int    i, from;
    string str;
    
    str = "";
    if (line > 0)
    {
	from = line - 10;
	if (from < 0)
	    from = 0;
	for (i=from; i<line; i++)
	    str += sprintf("%2d:%-70s\n", i, lines[i]);
    }
    player->catch_tell(str, 1);
}

nomask void
edit(string ffun, string str)
{
    finished_fun = ffun;
    calling_ob   = previous_object();
    player       = this_player();
    
    if (str)
    {
	lines = explode(str, "\n");
	line  = sizeof(lines);
    }
    else
    {
	lines = ({ });
	line  = 0;
    }
    player->catch_tell("Text editor. Use ~? or #? for help.\n", 1);
    display();
    player->catch_tell(sprintf("%2d:",line), 1);
    input_to("input");
}

static nomask void
checkrange()
{
    if (line < 0)
	line = 0;
    else if (line > sizeof(lines))
	line = sizeof(lines);
}

static nomask void
delete_lines(string arg)
{
    int from, to, erg;
    
    erg = sscanf(arg,"%d %d", from, to);
    if (erg == 1)
    {
	if (from < 0 || from >= sizeof(lines))
	{
	    player->catch_tell("That line does not exist.\n", 1);
	    return;
	}
	lines = exclude_array(lines, from, from);
	player->catch_tell("Deleted line " + from + ".\n", 1);
	return;
    }
    if (erg == 2)
    {
	if (from > to || from < 0 || to < 0 || from >= sizeof(lines) ||
	    to >= sizeof(lines))
	{
	    player->catch_tell("That's not possible.\n", 1);
	    return;
	}
	lines = exclude_array(lines, from, to);
	player->catch_tell("Deleted lines " + from + " to " + to + ".\n", 1);
	return;
    }
    player->catch_tell("Illegal arguments.\n", 1);
}

static nomask void
input(string str)
{
    int    i, sz;
    string cmd, arg, text, *split;
    
    if (!player)
    {
	destruct_object();
	return;
    }
	
    if (strlen(str) > 1 && (str[0] == '~' || str[0] == '#'))
    {
	cmd = extract(str, 1, 1);
	arg = extract(str, 2);

	switch (cmd) 
	{
	  case "?":
	      player->
		  catch_tell("Editor Commands (~ can be replaced with #):\n\n"+
			     "**      Finish text.\n"+
			     "~q      Cancel text.\n"+
			     "~l      Show complete text.\n"+
			     "~L      Show complete text with line numbers.\n"+
			     "~a x    Append after line x.\n"+
			     "~i x    Insert before line x.\n"+
			     "~A      Append at end of text.\n"+
			     "~I      Insert before top of text.\n"+
			     "~d x    Delete Line x.\n"+
			     "~d x y  Delete Lines from x to y.\n", 1);
	      break;
	  case "L":
	      str = "";
	      for (i=0, sz=sizeof(lines); i<sz; i++)
		  str += sprintf("%2d:%-74s\n", i, lines[i]);
	      player->catch_tell(str, 1);
	      break;
	  case "A":
	      line = sizeof(lines);
	      display();
	      break;
	  case "I":
	      line = 0;
	      display();
	      break;
	  case "a":
	  case "i":
	      if (arg)
		  sscanf(arg, "%d", line);
	      checkrange();
	      display();
	      break;
	  case "l":
	      player->catch_tell(implode(lines, "\n") + "\n", 1);
	      break;
	  case "d":
	      delete_lines(arg);
	      checkrange();
	      break;
	  case "q":
	      call_other(calling_ob, finished_fun, 0);
	      destruct_object();
	      return;
	  default:
	      player->catch_tell("*Unknown command*\n", 1);
	}
	player->catch_tell(sprintf("%2d:",line), 1);
	input_to("input");
	return;
    }
    if (str == "**") 
    {
	text = implode(lines, "\n") + "\n";
	call_other(calling_ob, finished_fun, text);
	destruct_object();
	return;
    }
    if (strlen(str) > 78)
    {
	str = break_string(str, 78, 0);
	split = explode(str, "\n");
    }
    else
	split = ({ str });
    
    if (!sizeof(lines))
	lines = split;
    else
	lines = lines[..line - 1] + split + lines[line..];
    line += sizeof(split);
    player->catch_tell(sprintf("%2d:",line), 1);
    input_to("input");
}
