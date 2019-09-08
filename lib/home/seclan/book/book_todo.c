inherit "/base/book";

#include <event.h>
#include <skills.h>
#include <macros.h>
#include <language.h>
#include <properties.h>

#define PAGESIZE 30 /* In lines */

static void
create_book()
{
    set_name("book");
    add_adj("grabbled");
    add_adj("dog-eared");
    add_adj("used");
    set_short("worn book");
    set_pshort("worn books");
    set_long_bs("This handy little book has been in permanent duty since its "+
	     "creation. Therefore it looks used and grabbled. It has "+
	     "spots everywhere on its cover and dog-ears decorate pages "+
	     "with possibly important notices.\n");
    set_maxpage(file_size("/home/seclan/ToDos")/2);
    set_language(SK_QUENYA);
    set_title("' Seclan's ToDo's '");
    add_lwvs(0, 900, 600, 8);
    set_value(5);
}

/*
 * Function name: read_cmd
 * Description:   called by cmdsoul if player reads the book
 * Arguments:     str - what should be read (e.g. which page)
 * Returns:       1 if success, -1 else
 * ATTENTION: Changed readfile to /home/seclan/ToDos
 */
int
read_cmd(string str)
{
    int    page, i;
    object tp;
    string *read_lines;

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
    read_lines = read_lines("/home/seclan/ToDos",(page-1)*PAGESIZE+1,PAGESIZE);
    tell_objects(LISTEN(tp) - ({ tp }),
		 ({ QCTNAME(tp), " reads in ", QNAME(this_object()), ".\n" }),
		 MSG_SEE, tp);
    if (page > 50)
	tp->catch_tell("That is strange! This book cannot have more than "+
		       "four dozens of pages,\nbut now you are on page "
		       + page + " and there are still many pages left.\n");
    if (!sizeof(read_lines))
    {
	tp->catch_tell("Page " + page + " is empty.\n");
	return 1;
    }
    tp->catch_tell("\nPage " + page + ":\n");
    for (i = 0; i < sizeof(read_lines); i++)
    {
	if (!(str = LANG_MOD(read_lines[i], language, tp)))
	{
	    tp->catch_tell("This language is beyond your comprehension.\n");
	    return 1;
	}
	else
	    tp->catch_tell(str+"\n");
    }
    return 1;
}
