#include <access.h>
#include <kernel.h>
#include <net.h>
#include <status.h>
#include <trace.h>

private int    creation_count, error_stack, *threadmark;
private object prev, next, sysd, global;

nomask object call_object(string objname);

nomask int
_Q_destruct()
{
  return _ADMIN;
}

nomask void
_F_set_prev(object p)
{
  if (previous_program() == _DRIVER)
    prev = p;
}

nomask void
_F_set_next(object n)
{
  if (previous_program() == _DRIVER)
    next = n;
}

nomask object
_Q_next()
{
  return next;
}

nomask int *
thread()
{
  if (!threadmark)
    threadmark = ({ });
  return threadmark;
}

/******************************************************************************
 * NAME:	insert_object()
 * DESCRIPTION:	put object in object chain
 */
private void
do_insert(object o)
{
  if (!o)
    return;
    
  if (prev)
    {
      o->_F_set_prev(prev);
      prev->_F_set_next(o);
    }
  prev = o;
}

nomask void
insert_object(object o)
{
  if (!PRIVILEGED())
    return;
  do_insert(o);
}

/******************************************************************************
 * NAME:	delete_object()
 * DESCRIPTION:	delete object from object chain
 */
private void
do_delete(object o)
{
  int    i;
  object p, n, *u;
    
  if (!o)
    return;
  if (prev == o)
    {
      prev = o->_Q_prev();
      return;
    }
  if (!(p = o->_Q_prev()) || !(n = o->_Q_next()))
    return;
  p->_F_set_next(n);
  n->_F_set_prev(p);

  if (++creation_count > _SWAPCHUNK)
    {
      u = users();
      for (i=sizeof(u); i--;)
	u[i]->catch_tell("\n* Cleanup in 10 seconds *\n\n");
      creation_count = 0;
      call_out("swapper", 10);
    }
}

static nomask void
swapper()
{
  swapout();
}

nomask void
delete_object(object o)
{
  if (!PRIVILEGED())
    return;
  do_delete(o);
}


/************************************************************************/

mapping editors;

/*
 * Function name: load_file_editor
 * Description:   read the editor storage file
 * Arguments:     path - the path to read
 * Return:        mapping, file : editor
 */
nomask mapping
load_file_editor(string path)
{
  if (!restore_object(path + _EDITORLOG) || !editors)
    editors = ([ ]);
	
  return editors;
}

private void
do_write_file_editor(string file, string euid)
{
  int    i, j;
  string *dir;

  i = strlen(file) - 1;
  while (i>=0 && file[i] != '/') i--;
    
  if (!restore_object(file[..i] + _EDITORLOG) || !editors)
    editors = ([ ]);
  else if (sizeof(dir = get_dir(file[..i] + "*")[0]) != map_sizeof(editors))
    {
      dir = map_indices(editors) - dir;
      for (j=0; j<sizeof(dir); j++)
	editors[dir[j]] = 0;
    }

  editors[file[i+1..]] = euid;
  save_object(file[..i] + _EDITORLOG);
}

/*
 * Function name: write_file_editor
 * Description:   update the editor of a file
 * Arguments:     file - the file that has been edited
 *                euid - effective user id of the editor
 */
nomask void
write_file_editor(string file, string euid)
{
  if (previous_program() != _AUTO)
    error("Illegal call to write_file_editor");
  do_write_file_editor(file, euid);
}


/******************************************************************************
 * NAME:	initialize
 * DESCRIPTION:	called once at game startup
 */
static nomask void
initialize()
{
  int    i, sz, t, l;
  string file, error, *files;
  object o;

  send_message("DGD " + status()[ST_VERSION] + "\n");
  send_message("Initializing...\n");

  compile_object(_AUTO);
  compile_object(_SYSTEMD);
  compile_object(_GLOBAL);
  sysd = find_object(_SYSTEMD);
  global = find_object(_GLOBAL);
  compile_object(_ACCESSD);
  compile_object(_LOGIN);

  send_message("\nRemoving /tmp files\n");
  files = get_dir("/tmp/*")[0];
  for (i=0, sz=sizeof(files); i<sz; i++)
    remove_file(files[i]);
    
  send_message("\nLoading init file /kernel/data/preload\n");
  files = explode(read_file("/kernel/data/preload"), "\n");

  for (i=0, sz=sizeof(files); i<sz; i++)
    {
      file = files[i];
      if (file && (l = strlen(file)) && file[0] != '#')
	{
	  if (file[(l-1)..] == "\r") /* windows cr */
	  {
	      file = file[..(l-2)];
	      l--;
	  }
	  if (!l) continue;
 
	  send_message("Preloading: " + file);
	  t = time();
	  if (file[l-2..] == ".c")
	    file = file[..(l-3)];
	  error = catch(o = compile_object(file));
	  if (error != nil)
	    send_message("\n " + error + "\n");
	  else
	    {
	      call_other(o, "???"); /* force creation */
	      send_message(" - " + (time() - t) + " sec.\n");
	    }
	}
    }   
  send_message("Setting up ipc.\n");
}

/******************************************************************************
 * NAME:	restored
 * DESCRIPTION:	called when started with dumpfile
 */
static nomask void
restored()
{
}

/******************************************************************************
 * NAME:	path_read()
 * DESCRIPTION:	handle an editor read path
 */
nomask string
path_read(string path)
{
  object player;

  if (!(player = global->query_this_player()))
    return nil;
  if (path && strlen(path) && path[0] != '/')
    path = player->query_path() + "/" + path;
  if (!_ACCESS_READ(path, player, "path_read"))
    return nil;
  return path;
}

/******************************************************************************
 * NAME:	path_write()
 * DESCRIPTION:	handle an editor write path
 */
nomask string
path_write(string path)
{
  object player;

  if (!(player = global->query_this_player()))
    return nil;
  if (path != "" && path[0] != '/') 
    path = player->query_path() + "/" + path;
  if (!_ACCESS_WRITE(path, player, "path_write"))
    return nil;
  do_write_file_editor(path, player->geteuid());
  write_file("/syslog/EDITOR", path + " edited by " +
	     player->query_name() + "\n");
  return path;
}

/******************************************************************************
 * NAME:	call_object()
 * DESCRIPTION:	translate a call_other string
 */
nomask object
call_object(string objname)
{
  object o;

  if (!prev)
    /* startup or if driver object was accidently destructed */
    {
      rlimits(-1; -1)
	{
	  if (!(o = find_object(_AUTO)))
	    o = compile_object(_AUTO);
	  next = prev = o;
	  while (o = prev->_Q_next())
	    prev = o;
	}
    }
  if (!objname)
    return nil;
  if (o = find_object(objname))
    return o;
  if (o = compile_object(objname))
    {
      do_insert(o);
      return o;
    }
  return nil;
}

/******************************************************************************
 * NAME:	inherit_program()
 * DESCRIPTION:	translate an inherit path and return object
 */
nomask object
inherit_program(string file, string path)
{
  object o;
  if (path[0] != '/')	path = file + "/../" + path;
  o = find_object(path);
  if (o == nil) o = compile_object(path);
  return o;
}

/******************************************************************************
 * NAME:	include_file()
 * DESCRIPTION:	translate an include path
 */
nomask string
include_file(string file, string path)
{
  if (path[0] != '/')
    return file + "/../" + path;
  return path;
}

/******************************************************************************
 * NAME:	recompile()
 * DESCRIPTION:	care for out of date inheritance (then destruct obj)
 */
static nomask void
recompile(object obj)
{
  if (obj)
    {
      send_message("Recompiling: " + object_name(obj) + "\n");
      /* don't use remove_object here, might cause infinite loops */
      do_delete(obj);
      destruct_object(obj);
    }
}

/*
 * NAME:	atomic_error()
 * DESCRIPTION:	log a runtime error in atomic code
 */
static void atomic_error(string str, int atom, int ticks)
{
  mixed **trace;
  string line, function, progname, objname;
  int i, sz, len;
  object obj;

  trace = call_trace();
  sz = sizeof(trace) - 1;

  str += " [atomic]\n";

  for (i = atom; i < sz; i++) {
    progname = trace[i][TRACE_PROGNAME];
    len = trace[i][TRACE_LINE];
    if (len == 0) {
      line = "    ";
    } else {
      line = "    " + len;
      line = line[strlen(line) - 4 ..];
    }

    function = trace[i][TRACE_FUNCTION];
    len = strlen(function);
    if (progname == _AUTO && i != sz - 1 && len > 3) {
      switch (function[.. 2]) {
      case "bad":
      case "_F_":
      case "_Q_":
	continue;
      }
    }
    if (len < 17) {
      function += "                 "[len ..];
    }

    objname = trace[i][TRACE_OBJNAME];
    if (progname != objname) {
      len = strlen(progname);
      if (len < strlen(objname) && progname == objname[.. len - 1] &&
	  objname[len] == '#') {
	objname = objname[len ..];
      }
      str += line + " " + function + " " + progname + " (" + objname +
	")\n";
    } else {
      str += line + " " + function + " " + progname + "\n";
    }
  }

  send_message(str);
}

/******************************************************************************
 * NAME:	compile_error()
 * DESCRIPTION:	return the name of a compile time error log
 */
static nomask void
compile_error(string file, int line, string err)
{
  object player;
    
  send_message("---------------------------------------------------------" +
	       "--------------\n" + ctime(time()) + ", compile error\n" +
	       file + ", " + line + ": " + err + "\n");
    
  player = global->query_this_player();
  if (error_stack < 4)
    {
      if (player != nil && !player->query_npc() &&
	  _ACCESS_LEVEL(player->geteuid()) > 0)
	{
	  rlimits(30; 100000)
	    {
	      player->catch_tell("Failed to compile:\n " +
				 file + ", " + line + ": " + err + "\n");
	    }
	}
      else 
	{
	  if (sysd == nil) sysd = find_object(_SYSTEMD);
	  if (sysd != nil) sysd->count_error(1, file, 0);
	}
    }
  error_stack = 0;
}

/******************************************************************************
 * NAME:	runtime_error()
 * DESCRIPTION:	log a runtime error
 */
static nomask void
runtime_error(string error, int caught, int ticks)
{
  int    i, sz, line, len;
  string progname, objname, function, out, str;
  mixed  **trace;
  object player;
    
  if (strlen(error) > 17 && error[..16] == "Failed to compile")
    return;
    
  if (caught)
    send_message("-----------------------------------------------------" +
		 "------------------\n" + ctime(time()) +
		 ", runtime error, ticks left: " + ticks +
		 " *caught*\n" + error + "\n");
  else
    send_message("-----------------------------------------------------" +
		 "------------------\n" + ctime(time()) +
		 ", runtime error, ticks left: " + ticks +
		 "\n" + error + "\n");
    	   
  trace = call_trace();
    
  if ((sz = sizeof(trace) - 1) == 0)
    return;
	
  if (sysd == nil) sysd = find_object(_SYSTEMD);
  if (sysd != nil) sysd->count_error(0, trace[sz-1][TRACE_PROGNAME],
				     trace[sz-1][TRACE_FUNCTION]);
			  
  out = "";
  for (i=0; i<sz; i++)
    {
      progname = trace[i][TRACE_PROGNAME];
      function = trace[i][TRACE_FUNCTION];

      if (progname == _AUTO && strlen(function) > 3)
	{
	  switch (function[0 .. 2])
	    {
	    case "bad":
	      progname = trace[i - 1][TRACE_PROGNAME];
	      function = trace[i - 1][TRACE_FUNCTION];
	    case "_F_":
	    case "_Q_":
	      continue;
	    default:
	      break;
	    }
	}

      objname  = trace[i][TRACE_OBJNAME];
      line     = trace[i][TRACE_LINE];
	    
      if (line == 0)
	str = "      --";
      else
	{
	  str = "        " + line;
	  str = str[strlen(str)-8..];
	}
	
      str += " " + function + " ";
      len = strlen(function);
      if (len < 17)
	str += "                 "[len ..];
	    
      str += " " + progname;
      if (progname != objname)
	{
	  len = strlen(progname);
	  if (len < strlen(objname) && progname == objname[.. len - 1])
	    str += " (" + objname[len ..] + ")";
	  else
	    str += " (" + objname + ")";
	}
      out += (str + "\n");
    }
  send_message(out);
    
  player = global->query_this_player();
    
  if (error_stack < 4 && player != nil && !player->query_npc())
    {
      if (_ACCESS_LEVEL(player->geteuid()) < 1)
	{
	  rlimits(30; 100000)
	    {
	      player->catch_tell("Your significant mind notices " +
				 "the fabric of space.\n");
	    }
	}
      else
	{
	  rlimits(30; 100000)
	    {
	      player->catch_tell("Runtime error:\n " + 
				 objname + ".c, " + line + ": " +
				 error + "\n");
	    }
	}
    }
  error_stack = 0;
}

/******************************************************************************
 * NAME:	compile_rlimits()
 * DESCRIPTION:	is changing rlimits allowed at compiletime?
 */
nomask int compile_rlimits(string filename)
{
  if (strlen(filename) < 8)
    return 0;
  if (filename[..7] == "/kernel/")
    return 1;
  return 0;
}

/******************************************************************************
 * NAME:	runtime_rlimits()
 * DESCRIPTION:	is changing rlimits allowed at runtime?
 */
nomask int runtime_rlimits(object obj, int stack, int ticks)
{
  if (stack < 0 || ticks < 0)
    return 0;
  return 1;
}

/******************************************************************************
 * NAME:	remove_program()
 * DESCRIPTION:	called if last reference of an object is gone
 */
nomask void
remove_program(string objname, int timestamp)
{
}

/******************************************************************************
 * NAME:	interrupt()
 * DESCRIPTION:	called if external TERM signal occurs
 */
nomask void
interrupt()
{
  shutdown();
}

/******************************************************************************
 * NAME:	save_data()
 * DESCRIPTION:	save all needed data to disk (e.g. all rooms)
 */
nomask void
save_data()
{
  int     i, j;
  mapping data, tmp;
  mixed   ind1, ind2;
    
  if (!PRIVILEGED())
    return;
  data = _ROOMD->query_room_map();
  ind1 = map_indices(data);
  for (i=sizeof(ind1); i--;)
  {
      tmp = data[ind1[i]];
      ind2 = map_indices(tmp);
      for (j=sizeof(ind2); j--;)
	      tmp[ind2[j]]->save();
  }
}

/******************************************************************************
 * NAME:	prepare_dump()
 * DESCRIPTION:	destruct precompiled objects to force new version
 */
nomask void
prepare_dump()
{
  int    i;
  object *pre;
    
  if (!PRIVILEGED())
    return;
  // pre = status()[ST_PRECOMPILED];
  // for (i=sizeof(pre); i--;)
  //   destruct_object(pre[i]);
}

#ifdef NET
/******************************************************************************
 * NAME:	send_message()
 * DESCRIPTION:	send to stdout
 */
nomask void
message(string str)
{
  /*if (object_name(previous_object())[..11] == "/kernel/net/")*/
    send_message(str);
}
#else
/******************************************************************************
 * NAME:	telnet_connect()
 * DESCRIPTION:	return a user object for telnet connection
 */
static object
telnet_connect()
{
  object o;

  o = call_object(_USER);
  o = clone_object(o);
  do_insert(o);
  return o;
}

/******************************************************************************
 * NAME:	binary_connect()
 * DESCRIPTION:	return a user object for binary connection
 */
static object
binary_connect()
{
  object o;

  o = call_object(_BINARY);
  o = clone_object(o);
  do_insert(o);
  return o;
}
#endif /* NET */
