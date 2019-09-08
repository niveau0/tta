inherit "object";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static int    maxpage, language;
static string bookdir, title;

/*
 * Function name: create_book
 * Description:   creates and initializes the book
 */
static void
create_book()
{
}

/*
 * Function name: create_object
 * Description:   std constructor
 */
static nomask void
create_object()
{
    add_prop(CONT_CLOSED, 1);
    add_prop(OBJ_MATERIAL, M_PAPER);
    create_book();
}

/*
 * Function name: set_maxpage
 * Description:   set maximum pages
 * Arguments:     mp - number of pages
 */
void
set_maxpage(int mp)
{
    maxpage = mp;
}

/*
 * Function name: set_bookdir
 * Description:   set directory where the book pages are
 * Arguments:     d - directory path
 */
void
set_bookdir(string d)
{
    bookdir = d;
}

/*
 * Function name: query_bookdir
 * Description:   give bookdir
 * Returns:       string
 */
string
query_bookdir()
{
    return bookdir;
}

/*
 * Function name: set_language
 * Description:   set the language in which the book was written
 * Arguments:     l - language skillnumber
 */
void
set_language(int l)
{
    language = l;
}

/*
 * Function name: query_language
 * Description:   give language skill
 * Returns:       int
 */
int
query_language()
{
    return language;
}

/*
 * Function name: set_title
 * Description:   set a title for the book
 * Arguments:     t - title string
 */
void
set_title(string t)
{
    title = t;
}

/*
 * Function name: query_title
 * Description:   get title, modified by language
 * Arguments:     tp - which player is reading
 */
string
query_title(object tp)
{
    string str;
    
    if (!tp)
	return title;
    if (!(str = LANG_MOD(title, language, tp)))
	return "unreadable";
    return str;
}

/*
 * Function name: short
 * Description:   modified short() function for handling closed/open books
 * Returns:       string - the proper shortdescription
 */
string
short()
{
    if (query_prop(CONT_CLOSED))
	return "closed " + ::short();
    return ::short();
}

/*
 * Function name: plural_short
 * Description:   modified plural_short() function like 'short' above
 * Returns:       string - the proper pshortdesc
 */
string
plural_short()
{
    if (query_prop(CONT_CLOSED))
	return "closed " + ::plural_short();
    return ::plural_short();
}

/*
 * Function name: long
 * Description:   modified long() function for handling description of books
 * Returns:       string - the proper longdesc
 */
string
long()
{
    return ::long() + "It's titled " + query_title(this_player()) + "\n";
}

/*
 * Function name: read_cmd
 * Description:   called by cmdsoul if player reads the book
 * Arguments:     str - what should be read (e.g. which page)
 * Returns:       1 if success, -1 else
 */
int
read_cmd(string str)
{
    int    page;
    object tp;

    tp = this_player();
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("You should open the " + ::short() + " first.\n");
	return -1;
    }
    if (!str || !sscanf(str, "page %s", str) || !(page = LANG_WNUM(str)))
    {
	tp->catch_tell("Usage: read page <number> of <book>\n");
	return -1;
    }
    if (page > maxpage)
    {
	tp->catch_tell("There are only " + maxpage + " pages.\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " reads in ", QNAME(this_object()), ".\n" }),
		 MSG_SEE, tp);
    if (file_size(bookdir + "/page" + page) < 1)
    {
	tp->catch_tell("Page " + page + " is empty.\n");
	return 1;
    }
    tp->catch_tell("\nPage " + page + ":\n");
    str = read_file(bookdir + "/page" + page);
    if (!(str = LANG_MOD(str, language, tp)))
    {
	tp->catch_tell("Ouch, you cannot decipher a single letter!\n");
	return 1;
    }
    smore(str, tp);
    return 1;
}

/*
 * Function name: open_cmd
 * Description:   opens a book and destroys the seal if present
 * Returns:       1 if success, -1 else
 */
int
open_cmd()
{
    object tp;

    tp = this_player();
    
    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But the " + ::short() + " is already open.\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " opens ", QNAME(this_object()), ".\n" }),
		 MSG_SEE, tp);
    tp->echo("You open the " + ::short() + ".\n");
    add_prop(CONT_CLOSED, 0);
    return 1;
}

/*
 * Function name: close_cmd
 * Description:   closes the book
 * Returns:       1 if successful, -1 else
 */
int
close_cmd(string argument)
{
    object tp;
    
    tp = this_player();
    
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But the " + ::short() + " is already closed.\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({tp}),
		 ({ QCTNAME(tp), " closes ", QNAME(this_object()), ".\n" }),
		 MSG_SEE, tp);
    tp->echo("You close the " + ::short() + ".\n");
    add_prop(CONT_CLOSED, 1);
    return 1;
}
    
/*
 * Function name: light_cmd
 * Description:   handles the attempt to light/burn the book.
 *                maybe called, if book/player is hit by a fire-attack
 * Returns:       1 if success, -1 else
 */
int
light_cmd()
{
    object tp;
    
    tp = this_player();
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " burns ", QNAME(this_object()),
		    " to dust.\n" }), MSG_SEE, tp);
    tp->catch_tell("You burn the " + ::short() + " to dust.\n");
    remove_object();
    return 1;
}
