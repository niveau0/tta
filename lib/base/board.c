inherit "object";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <config.h>
#include <properties.h>

private string newsgroup;

/*
 * Function name: create_board
 * Description:   redefinable constructor
 */
static void
create_board()
{
}

/*
 * Function name: create_object
 * Description:   standard constructor
 */
static nomask void
create_object()
{
    add_name("board");
    set_name("pinboard");
    create_board();
    add_prop(OBJ_NO_GET, 1);
    set_lwvs(0, 100000, 110000, 80);
}

/*
 * Function name: set_group
 * Description:   set the newsgroup this board should be connected too.
 *                must be a mortalboard name like: mortal.<groupname>
 * Arguments:     g - newsgroup
 */
static nomask void
set_group(string g)
{
    if (!g || sscanf(g, "%*s.%*s") != 2)
	error("Bad groupname");
    newsgroup = g;
}

/*
 * Function name: query_group
 * Description:   returns the connected newsgroup name
 * Returns:       string
 */
nomask string
query_group()
{
    return newsgroup;
}

nomask int
read_cmd(string arg)
{
    int    note;
    string str;
    object tp;

    tp = this_player();
    if (!arg)
    {
	NEWS_MASTER->board_list(newsgroup, 0, tp);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " takes a close look at ",
			    QTNAME(this_object()), ".\n" }), MSG_SEE, tp);
	return 1;
    }
    tp = this_player();
    if (sscanf(arg, "note %d", note))
    {
	NEWS_MASTER->board_article(newsgroup, (string)note, tp);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " reads something at ",
			    QTNAME(this_object()), ".\n" }), MSG_SEE, tp);
	return 1;
    }
    if (sscanf(arg, "%*d-%*d") == 2)
    {
	NEWS_MASTER->board_list(newsgroup, arg, tp);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " takes a close look at ",
			    QTNAME(this_object()), ".\n" }), MSG_SEE, tp);
	return 1;
    }
    tp->catch_tell("Usage: read note <number> of <board>\n" +
		   "       read <board>\n" +
		   "       read <num>-<num> of <board>");
    return -1;
}

nomask int
write_cmd()
{
    object tp;

    tp = this_player();
    NEWS_MASTER->post_note(query_group(), tp);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts to write a note.\n" }), MSG_SEE, tp);
    return 1;
}
