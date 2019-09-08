#include <access.h>
#include <config.h>
#include <macros.h>

#define DIR   "/system/news"
#define OPT_H 1

nomask void input_article();
static nomask void article_summary();

static string  pldir,                     /* player directory */
               currnote,                  /* current article */       
               currgrp,                   /* currently chosen group */       
               *grpindex;                 /* group names */
static object  player;                    /* reading player */
static mapping news,                      /* unread articles */
               groups;                    /* all articles (master object) */

int            options;
mapping        data;

static nomask void
create()
{
    int i;
    
    if (IS_CLONE)
	return;
    groups = ([ ]);
    grpindex = get_dir(DIR + "/*");
    for (i=sizeof(grpindex); i--;)
	if (file_size(DIR + "/" + grpindex[i]) == -2)
	    groups[grpindex[i]] = get_dir(DIR + "/" + grpindex[i] + "/*");
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}

/*****************************************************************************
 *
 */
nomask mapping
query_group_contents()
{
    return groups;
}

nomask string *
query_groups()
{
    return grpindex[..];
}

nomask string
query_note_file(string grp, string note)
{
    note = extract("00000000", 0, 7 - strlen(note)) + note;
    if (file_size(DIR + "/" + grp + "/" + note) > 0)
	return note;
    return 0;
}

nomask string
query_next_article(string grp)
{
    int    sz, num;
    string note;
    
    if (!pointerp(groups[grp]))
	return 0;
    if (!(sz = sizeof(groups[grp])))
	return "00000001";
    sscanf(groups[grp][sz-1], "%d", num);
    note = (string)++num;
    note = extract("00000000", 0, 7 - strlen(note)) + note;
    return note;
}

/*****************************************************************************
 *
 */
nomask void
add_new_group(string grp)
{
    if (MASTER_OB(previous_object()) != NEWS_MASTER)
	return;
    make_dir(DIR + "/" + grp);
    if (!groups[grp])
	groups[grp] = ({ });
    grpindex = get_dir(DIR + "/*"); 
}

nomask void
add_new_note(string grp, string note)
{
    if (MASTER_OB(previous_object()) != NEWS_MASTER)
	return;
    if (!groups[grp])
	return;
    groups[grp] += ({ note });
}

nomask void
remove_note(string grp, string note)
{
    if (MASTER_OB(previous_object()) != NEWS_MASTER)
	return;
    if (!groups[grp])
	return;
    groups[grp] -= ({ note });
}

/*****************************************************************************
 *
 */
static nomask void
delete_note()
{
    string path, *lines;

    path = DIR + "/" + currgrp + "/" + currnote;
    if (!currnote)
    {
	player->catch_tell("First you need to select a note.\n", 1);
	return;
    }
    lines = read_lines(path, 1, 2);
    if (sizeof(lines) < 1)
    {
	player->catch_tell("There is no such note.\n", 1);
	return;
    }
    if ((strlen(currgrp) < 4 || currgrp[..3] != "log.") &&
	lines[1] != player->query_name() &&
	_ACCESS_LEVEL(geteuid(player)) < _CREATOR)
    {
	player->catch_tell("You are not allowed to delete this note.\n", 1);
	return;
    }
    remove_file(path);
    MASTER->remove_note(currgrp, currnote);
    currnote = 0;
    player->catch_tell("Ok.\n", 1);
}

/*****************************************************************************
 *
 */
static nomask void
display_article(string note)
{
    int    pos;
    string str, *lines, *grp;

    if (note)
    {
	if (!(currnote = query_note_file(currgrp, note)))
	{
	    player->catch_tell("No such article.\n", 1);
	    input_article();
	    return;
	}
    }
    else
    {
	if (!sizeof(news[currgrp]))
	{
	    grp = map_indices(news);
	    if ((pos = member_array(currgrp, grp)) == -1 ||
		pos == sizeof(grp) - 1)
	    {
		player->catch_tell("No more unread news.\n", 1);
		input_article();
		return;
	    }
	    currgrp = grp[pos + 1];
	    currnote = 0;
	    article_summary();
	    input_article();
	    return;
	}
	currnote = news[currgrp][0];
    }
    lines = read_lines(DIR + "/" + currgrp + "/" + currnote, 1, 50);
    if (options & OPT_H)
    {
	str = sprintf("--------------------------------------------------" +
		      "----------------------------\n" +
		      "Subject: %-40s\n" +
		      "Author : %-40s\n" +
		      "Date   : %-40s\n" +
		      "--------------------------------------------------" +
		      "----------------------------\n",
		      lines[0], lines[1], lines[2]);
    }
    else
	str = "";
    
    smore(str + implode(lines[3..], "\n") + "\n", player, "input_article");
    /* mark note as read */
    if (!pointerp(data[currgrp]))
	data[currgrp] = ({ 0, ({ currnote }) });
    else
	data[currgrp][1] |= ({ currnote });
    /* remove note from 'new' notes */
    if (news[currgrp])
	news[currgrp] -= ({ currnote });
}

nomask void
board_article(string group, string note, object tp)
{
    string str, *lines;

    if (!tp || !group || IS_CLONE)
	return;
    
    if (!note || !(currnote = query_note_file(group, note)))
    {
	tp->catch_tell("You don't see such a note.\n");
	return;
    }
    lines = read_lines(DIR + "/" + group + "/" + currnote, 1, 50);
    str = "";
    smore(str + implode(lines[3..], "\n") + "\n", tp);
}

/*****************************************************************************
 *
 */
static nomask void
edit_article(string note)
{
    string *lines;

    if (!note || !(currnote = query_note_file(currgrp, note)))
    {
	player->catch_tell("No such article.\n", 1);
	input_article();
	return;
    }
    lines = read_lines(DIR + "/" + currgrp + "/" + currnote, 1, 50);
    if (lines[1] != player->query_name() &&
	_ACCESS_LEVEL(geteuid(player)) < _CREATOR)
    {
	player->catch_tell("You are not allowed to edit this note.\n", 1);
	input_article();
	return;
    }
    player->add_prop("_tmp_subject", lines[0]);
    EDIT("edit_finish", implode(lines[3..], "\n"));
}

nomask void
edit_finish(string str)
{
    if (!str || !strlen(str))
    {
	player->catch_tell("Cancelled.\n", 1);
	input_article();
	return;
    }
    str = (player->query_prop("_tmp_subject") + "\n" +
	   player->query_name() + "\n" + ctime(time()) + "\n" + str);
    player->add_prop("_tmp_subject", 0);
    remove_file(DIR + "/" + currgrp + "/" + currnote);
    write_file(DIR + "/" + currgrp + "/" + currnote, str);
    /* mark note as read */
    if (!data[currgrp]) 
	data[currgrp] = ({ 0, ({ currnote }) });
    else
	data[currgrp][1] |= ({ currnote });
    input_article();
}

/*****************************************************************************
 *
 */
static nomask void
list_notes(string arg)
{
    int    first, last, i, sz, num;
    string str, path, *notes, *lines;

    notes = MASTER->query_group_contents()[currgrp];
    
    if (!arg)
    {
	first = 1;
	last = 100000000;
    }
    else if (sscanf(arg, "%d-%d", first, last) != 2 || first > last)
    {
	player->catch_tell("Wrong syntax.\n", 1);
	input_article();
	return;
    }
    path = DIR + "/" + currgrp + "/";
    str  = "";
    for (i=0, sz=sizeof(notes); i<sz; i++)
    {
	if (!sscanf(notes[i], "%d", num) || num < first || num > last)
	    continue;
	lines = read_lines(path + notes[i], 1, 2);
	if (sizeof(lines) < 2)
	    continue;
	str += sprintf("%5d: %_/-50s__[%" + strlen(lines[1]) + "s]\n",
			num, lines[0], lines[1]);
    }
    smore(str, player, "input_article");
}

nomask void
board_list(string group, string arg, object tp)
{
    int    first, last, i, sz, num;
    string str, path, *notes, *lines;
    
    if (!tp || !group || IS_CLONE)
	return;
    notes = query_group_contents()[group];
    
    if (!arg)
    {
	first = 1;
	last = 100000000;
    }
    else if (sscanf(arg, "%d-%d", first, last) != 2 || first > last)
    {
	tp->catch_tell("Wrong number of notes.\n");
	return;
    }
    path = DIR + "/" + group + "/";
    str  = "";
    for (i=0, sz=sizeof(notes); i<sz; i++)
    {
	if (!sscanf(notes[i], "%d", num) || num < first || num > last)
	    continue;
	lines = read_lines(path + notes[i], 1, 1);
	if (sizeof(lines) < 1)
	    continue;
	str += sprintf("%5d)  %-40s\n", num, lines[0]);
    }
    if (!strlen(str))
	tp->catch_tell("The are currently no notes.\n");
    else
	smore("You see the following notes:\n" + str, tp);
}

/*****************************************************************************
 * post a note. if object is no clone it was a call from mortal board
 */
nomask void
post_note(string group, object tp) 
{
    int flag;
    
    if (!IS_CLONE)
    {
	if (!tp)
	    error("Wrong call to post_note()");
	flag = 1;
    }
    else
	tp = player;
    
    tp->catch_tell("Subject: ", 1);
    tp->add_prop("_tmp_newsgroup", group);
    input_to("get_subject", 0, flag, tp);
}

/* flag is false if player uses newsreader */
static nomask void
get_subject(string str, int flag, object tp)
{
    player = tp;
    if (!str || !strlen(str))
    {
	player->catch_tell("Cancelled.\n", !flag);
	if (!flag)
	    input_article();
	return;
    }
    player->add_prop("_tmp_subject", str);
    player->add_prop("_tmp_newsflag", flag); 
    EDIT("post_finish", 0);
}

nomask void
post_finish(string str)
{
    int    flag;
    string note, group;

    player = this_player();

    flag = player->query_prop("_tmp_newsflag");
    currgrp = player->query_prop("_tmp_newsgroup");
    
    player->add_prop("_tmp_newsflag", 0);
    player->add_prop("_tmp_newsgroup", 0);
    
    if (!str || !strlen(str))
    {
	player->catch_tell("Cancelled.\n", !flag);
	if (!flag)
	    input_article();
	return;
    }
    str = (player->query_prop("_tmp_subject") + "\n" +
	   player->query_name() + "\n" + ctime(time()) + "\n" + str);
    player->add_prop("_tmp_subject", 0);
    
    if (!(note = MASTER->query_next_article(currgrp)))
    {
	player->catch_tell("Failed.\n", !flag);
	if (!flag)
	    input_article();
	return;
    }
    MASTER->add_new_note(currgrp, note);
    write_file(DIR + "/" + currgrp + "/" + note, str);
    if (!flag)
    {
	/* mark note as read */
	if (!data[currgrp]) 
	    data[currgrp] = ({ 0, ({ note }) });
	else
	    data[currgrp][1] |= ({ note });
	input_article();
    }
    else
	player->catch_tell("Ok.\n");
}

nomask void
post_log(string str, string subject, string group, object tp)
{
    string note;
    
    if (previous_program() != MISC_SOUL)
	error("Illegal call to post_log");
    str = subject + "\n" +  tp->query_name() + "\n" +
	  ctime(time()) + "\n" + str;
    group = "log." + group;
    if (!(note = MASTER->query_next_article(group)))
    {
	tp->catch_tell("Failed.\n");
	return;
    }
    MASTER->add_new_note(group, note);
    write_file(DIR + "/" + group + "/" + note, str);
}

/*****************************************************************************
 *
 */
static nomask void
set_options(string opt)
{
    switch (opt)
    {
      case "H":
	options ^= OPT_H;
	break;
      default:
	player->catch_tell("No such option. Current setting: " +
			   (options & OPT_H ? "H" : "") +
			 "\nPossible:\n  'H' (show header)\n", 1);
	return;
    }
    player->catch_tell("Ok.\n", 1);
}

/*****************************************************************************
 *
 */
static nomask void
article_summary()
{
    int     i, sz, cur;
    string  str, path, *ind, *lines;
    
    str  = "";
    ind  = news[currgrp];
    path = DIR + "/" + currgrp + "/";
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	lines = read_lines(path + ind[i], 1, 2);
	sscanf(ind[i], "%d", cur);
	if (sizeof(lines) < 2)
	    continue;
	str  += sprintf("%5d: %_/-50s__[%" + strlen(lines[1]) + "s]\n",
			cur, lines[0], lines[1]);
    }
    player->catch_tell(str, 1);
}

static nomask varargs int
group_summary(int flag)
{
    int     i, sz, ur, unread;
    string  str, cur, *ind;
    mapping grps;

    str  = "";
    grps = MASTER->query_group_contents();
    ind  = map_indices(grps);
    news = ([ ]);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	cur = ind[i];
	if (!data[cur])
	    news[cur] = grps[cur];
	else
	{
	    /* remove deleted messages from 'read' data */
	    data[cur][1] &= grps[cur];
	    if (data[cur][0]) /* not subscribed? */
		continue;
	    news[cur] = grps[cur] - data[cur][1];
	}
	if (!(ur = sizeof(news[cur])))
	{
	    news[cur] = 0;
	    continue;
	}
	if (flag)
	    unread += ur;
	else
	    str += sprintf("%3d %5s %-30s\n",
			   ur, "(" + sizeof(grps[cur]) + ")", cur);
    }
    if (flag)
	return unread;
    player->catch_tell(str, 1);
    return 0;
}

/*****************************************************************************
 *
 */
nomask void
input_group()
{
    player->catch_tell(MUD_ABBREV + " news: ", 1);
    input_to("get_input_group");
}

static nomask void
get_input_group(string str)
{
    int    i, sz;
    string arg, *ind, *grp;

    if (!str || !strlen(str))
    {
	if (!sizeof(ind = map_indices(news)))
	{
	    player->catch_tell("No more unread articles.\n", 1);
	    input_group();
	    return;
	}
	currgrp = ind[0];
	article_summary();
	input_article();
	return;
    }
    sscanf(str, "%s %s", str, arg);
    
    switch(str)
    {
      case "?":
	  player->catch_tell("<cr>        Enter next unread group\n" +
			     "c <group>   Create group\n" +
			     "j <group>   Jump to group\n" +
			     "g <group>   Goto group (same as jump)\n" +
			     "ls          List subscribed groups\n" +
			     "lu          List unsubscribed groups\n" +
			     "o <option>  Toggle options, possible: H\n" +
			     "q           Quit, exits newsreader\n" +
			     "U <group>   Mark group as unread\n" +
			     "R <group>   Mark group as read\n" +
			     "s <group>   Subscribe group\n" +
			     "u <group>   Unsubscribe group\n", 1);
	  input_group();
	  return;
      case "c":
	  if (_ACCESS_LEVEL(geteuid(player)) < _CREATOR)
	  {
	      player->catch_tell("Only the administration may add " +
				 "new groups.\n", 1);
	      input_group();
	      return;
	  }
	  if (!arg || !strlen(arg))
	  {
	      player->catch_tell("You must give a group name.\n", 1);
	      input_group();
	      return;
	  }
	  if (IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("Already exists.\n", 1);
	      input_group();
	      return;
	  }
	  MASTER->add_new_group(arg);
	  input_group();
	  return;
      case "j":
      case "g":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_group();
	      return;
	  }
	  currgrp = arg;
	  article_summary();
	  input_article();
	  return;
      case "ls":
	  grp = MASTER->query_groups();
	  for (i=0, sz=sizeof(grp); i<sz; i++)
	  {
	      if (data[grp[i]] && data[grp[i]][0])
		  grp[i] = 0;
	  }
	  grp -= ({ 0 });
	  smore(sprintf("% *78s", implode(grp, "\n")), player, "input_group");
	  return;
      case "lu":
	  grp = MASTER->query_groups();
	  for (i=0, sz=sizeof(grp); i<sz; i++)
	  {
	      if (!data[grp[i]] || !data[grp[i]][0])
		  grp[i] = 0;
	  }
	  grp -= ({ 0 });
	  smore(sprintf("% *78s", implode(grp, "\n")), player, "input_group");
	  return;
      case "o":
	  set_options(arg);
	  input_group();
	  return;
      case "q":
	  save_object(pldir + "/newsdata");
	  player->catch_tell("Ok.\n");
	  destruct_object();
	  return;
      case "U":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_group();
	      return;
	  }
	  if (data[arg])
	      data[arg][1] = ({ });
	  player->catch_tell("Ok.\n", 1);
	  group_summary();
	  input_group();
	  return;
      case "R":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_group();
	      return;
	  }
	  if (!data[arg])
	      data[arg] = ({ 1, get_dir(DIR + "/" + arg + "/*") });
	  else
	      data[arg][1] = get_dir(DIR + "/" + arg + "/*");
	  player->catch_tell("Ok.\n", 1);
	  group_summary();
	  input_group();
	  return;
      case "s":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_group();
	      return;
	  }
	  if (data[arg])
	      data[arg][0] = 0;
	  player->catch_tell("Ok.\n", 1);
	  input_group();
	  return;
      case "u":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_group();
	      return;
	  }
	  if (!data[arg])
	      data[arg] = ({ 1, ({ }) });
	  else
	      data[arg][0] = 1;
	  player->catch_tell("Ok.\n", 1);
	  input_group();
	  return;
    }
    player->catch_tell("No such command, use '?' for help.\n", 1);
    input_group();
}

/*****************************************************************************
 *
 */
nomask void
input_article()
{
    int    sz;
    string *articles;
    
    articles = MASTER->query_group_contents()[currgrp];
    if (sz = sizeof(articles))
    {
	int  c, f, l;
	
	if (currnote)
	    sscanf(currnote, "%d", c);
	sscanf(articles[0], "%d", f);
	sscanf(articles[sz-1], "%d", l);
	player->catch_tell(currgrp + " (" + f + "-" + l + ")" +
			   (c ? ", " + c : "") + ": ", 1);
    }
    else
	player->catch_tell(currgrp + " (No articles): ", 1);
    
    input_to("get_input_article");
}

static nomask void
get_input_article(string str)
{
    string arg;

    if (!str || !strlen(str))
    {
	display_article(0);
	return;
    }
    sscanf(str, "%s %s", str, arg);
    switch(str)
    {
      case "?":
	  player->catch_tell("D           Delete current note\n" +
			     "e <num>     Edit own note\n" +
			     "j <group>   Jump to group\n" +
			     "g <group>   Goto group (same as jump)\n" +
			     "l <num-num> List notes\n" +
			     "p           Post a new note\n" +
			     "q           Quit, return to group summary\n" +
			     "<num>       Read note number <num>\n" +
			     "<cr>        Show next unread note\n", 1);
	  input_article();
	  return;
      case "e":
	  edit_article(arg);
	  return;
      case "j":
      case "g":
	  if (!IN_ARRAY(arg, MASTER->query_groups()))
	  {
	      player->catch_tell("No such group.\n", 1);
	      input_article();
	      return;
	  }
	  currnote = 0;
	  currgrp = arg;
	  article_summary();
	  input_article();
	  return;
      case "p":
	  post_note(currgrp, 0);
	  return;
      case "l":
	  list_notes(arg);
	  return;
      case "D":
	  delete_note();
	  input_article();
	  return;
      case "q":
	  currnote = 0;
	  currgrp = 0;
	  group_summary();
	  input_group();
	  return;
    }
    if (sscanf(str, "%*d"))
    {
	display_article(str);
	return;
    }
    player->catch_tell("No such command, use '?' for help.\n", 1);
    input_article();
}

/*****************************************************************************
 * called from cmdsoul
 */
nomask varargs void
read_news(object pl, int flag)
{
    if (!pl || player)
    {
	destruct_object();
	return;
    }
    player = pl;
    pldir  = PLAYER_DIR(pl->query_name());
    restore_object(pldir + "/newsdata");
    if (!data)
	data = ([ ]);
    if (flag)
    {
	player->catch_tell("\nThere are " + group_summary(1) +
			   " unread news.\n");
	player = 0;
	options = 0;
	data = 0;
	return;
    }
    if (!IS_CLONE)
	return;
    player->catch_tell(" *------------------------------"+
		       "---------------------------------------------*\n"+
		       sprintf("%|77s\n", "'" + MUD_NAME + "' mudnews") +
		       " *------------------------------"+
		       "---------------------------------------------*\n");
    group_summary();
    input_group();
}
