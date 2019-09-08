/*********************************************************************
 * base.c
 * Base functions for the room creation
 */
static int     doculevel, changeflag;
static object  tp;
static mapping editors;

static nomask int save_this();
static nomask int docu(string arg);
static nomask int edit();
static nomask int view(string arg);
static nomask string view_result(int choice);

#include "border.c"
#include "climate.c"
#include "coordinates.c"
#include "description.c"
#include "documentation.c"
#include "edit.c"
#include "exit.c"
#include "height.c"
#include "item.c"
#include "jump.c"
#include "light.c"
#include "offer.c"
#include "parent.c"
#include "path.c"
#include "size.c"
#include "type.c"
#include "view.c"

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
	  return edit();
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
	  return save_this();
      }
      default:
	  return 0;
    }
}
