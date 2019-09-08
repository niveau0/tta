inherit "/base/object";

#include <macros.h>
#include <language.h>
#include <material.h>
#include <math.h>
#include <properties.h>
#include <skills.h>

#define OBJ_DAMAGE 40 // in percent

// fliegen noch raus, wenn das nach /base/book.c gehen sollte?
void set_title(string t);
void set_maxpage(int mp);
void set_bookdir(string d);
void set_language(int l);

static int    maxpage, language;
static string bookdir, title;

/*
 * Function name: create_book
 * Description:   creates and initializes the book
 */
static void
create_book()
{
    set_name("book");
    set_short("old book");
    set_pshort("old books");
    set_long("An old book with testy character ;).\n");
    set_maxpage(9);
    set_bookdir("/lib/book/poison");
    set_language(SK_QUENYA);
    set_title("-=+ Burn, baby, burn! +=-");
    add_lwvs(0, 900, 600, 8);
    set_value(5);
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

nomask string
mask_page(string str, int damage, int page, int height, int width)
{
    int i_old, length, tmp_height, tmp_width, x_dist, y_dist, z_dist;
    float scaled_damage;
    string masked_string;

    scaled_damage = (float) (width*height*maxpage*damage)/800.0;
		    /* 1/2*width*1/2*height*1/2*maxpage*damage/100
		     * Scales the damage according to the booksize.
		     */
    masked_string = "";
    length = strlen(str);
    z_dist = page < maxpage/2 ? page : maxpage - page;
    for (tmp_height=0; tmp_height < height; tmp_height++)
    { 
	y_dist = tmp_height < height/2 ? tmp_height : height - tmp_height;
	for (tmp_width=0; tmp_width < (width-1); tmp_width++)
	{
	    x_dist = tmp_width < width/2 ? tmp_width : width - tmp_width;
	    if ((str[i_old] == '\n') || (i_old >= length))
	    {
		// Filling the rest of the line.
		for ( ; tmp_width < (width-1); tmp_width++)
		{
		    x_dist = (tmp_width < width/2)
			     ? tmp_width : (width - tmp_width);
		    if ( (float) (x_dist*y_dist*z_dist) < scaled_damage)
			masked_string = masked_string + ".";//verkohlen
		    else
			masked_string = masked_string + " ";
		}
	    }
	    else
	    {
		// Masked or not masked?
		if ( (float) (x_dist*y_dist*z_dist) < scaled_damage)
		    masked_string = masked_string + ".";//verkohlen
		else
		{
		    masked_string = masked_string + "-";//uebernehmen
		    masked_string[strlen(masked_string)-1] = str[i_old];
		}
	    }
	    i_old++;
	}
	masked_string = masked_string + "\n";
    }
    return masked_string;
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
    int  length, i, height, width, maxwidth, page;
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
	tp->catch_tell("You cannot decipher a single letter!\n");
	return 1;
    }
    // get the height and wide of this page
    length = strlen(str);
    for (i=0; i < length; i++)
    {
	width++;
	if (str[i] == '\n')
	{
	    height++;
	    if (width > maxwidth)
		maxwidth = width;
	    width = 0;
	}
    }
    if (str[length-1] != '\n')
	height++;
    if (width > maxwidth)
	maxwidth = width;
    debug("seclan","maxwidth:"+maxwidth+" height:"+height);
    // destruct the output adequate to book's damage
    smore(mask_page(str, OBJ_DAMAGE, page, height, maxwidth), tp);
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
