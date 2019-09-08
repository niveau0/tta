inherit "object";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

string title, *pages;

static int maxpages, language;

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
    restore_object(MASTER);
    if (!pages)
	pages = allocate(maxpages);
    else if (sizeof(pages) > maxpages)
	maxpages = sizeof(pages);
    else if (sizeof(pages) < maxpages)
	pages += allocate(sizeof(pages) - maxpages);
    if (!title)
	title = "untitled";
}

/*
 * Function name: write_cmd
 * Description:   add/remove/change pages
 * Arguments:     str - where to write
 * Returns:       1 if success, -1 else
 */
nomask int
write_cmd(string str)
{
    int    page;
    object tp;

    tp = this_player();
    if (str == "title")
    {
	tp->catch_tell("New title: ", 1);
	input_to("set_title", 0, tp);
	return 1;
    }
    if (str && sscanf(str, "page %d", page))
    {
	if (page < 1 || maxpages < page)
	{
	    tp->catch_tell("There are no such page.\n");
	    return -1;
	}
	tp->add_prop("_tmp_edit_page", page-1);
	EDIT("edit_finish", pages[page-1]);
	return 1;
    }
    tp->catch_tell("Write on title or page <no>.\n");
    return -1;
}

/*
 * Function name: edit_finish
 * Description:   called to store a written page after editing
 * Arguments:     
 * Returns:       
 */
nomask void
edit_finish(string str)
{
    int    page;
    object tp;

    tp = previous_object()->query_editor();
    if (!tp)
	return;
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled.\n", 1);
	return;
    }
    page = tp->query_prop("_tmp_edit_page");
    if (page < 0 || sizeof(pages) <= page)
	return;
    pages[page] = str;
    tp->catch_tell("Ok.\n");
    save_object(MASTER);
}

/*
 * Function name: set_maxpages
 * Description:   set maximum pages
 * Arguments:     mp - number of pages
 */
static void
set_maxpages(int mp)
{
    maxpages = mp;
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
 * Description:   set the new book title
 * Arguments:     str - the title
 */
static nomask void
set_title(string t, object tp)
{
    if (!t || !tp)
	return;
    title = t;
    tp->catch_tell("Ok.\n");
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
    if (page > maxpages)
    {
	tp->catch_tell("There are only " + maxpages + " pages.\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " reads in ", QNAME(this_object()), ".\n" }),
		 MSG_SEE, tp);
    if (!pages[page-1] || strlen(pages[page-1]) < 1)
    {
	tp->catch_tell("Page " + page + " is empty.\n");
	return 1;
    }
    tp->catch_tell("\nPage " + page + ":\n");
    if (!(str = LANG_MOD(pages[page-1], language, tp)))
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
