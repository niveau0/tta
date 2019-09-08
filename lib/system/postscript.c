/* tools useful for creating postscript output */

#include <event.h>
#include <language.h>
#include <macros.h>

#define PS_PROLOG \
"%%BeginProlog\n"+\
"/D {def} bind def\n"+\
"/BD {bind def} bind def\n"+\
"\n"+\
"% Definitions:\n"+\
"/mm {72 mul 25.4 div} BD\n"+\
"/margin 10 mm D  /mediawidth 210 mm D  /mediaheight 297 mm D\n"+\
"/pagewidth mediawidth margin dup add sub D\n"+\
"/pageheight mediaheight margin dup add sub D\n"+\
"\n"+\
"% Open ranged page:\n"+\
"/RANGE {/height exch def /width exch def /yo exch def /xo exch def\n"+\
" mediawidth 2 div  mediaheight 2 div  translate\n"+\
" width height gt\n"+\
"  {90 rotate  pageheight width div  pagewidth height div  min dup scale}\n"+\
"  {pagewidth width div  pageheight height div  min dup scale} ifelse\n"+\
" width 2 div xo add neg  height 2 div yo add neg  translate} BD\n"+\
"\n"+\
"% Text:\n"+\
"/S {3 -1 roll M show} BD\n"+\
"/RS {gsave translate dup stringwidth pop neg 0 moveto show grestore} BD\n"+\
"/CS {gsave translate dup stringwidth pop 2 div neg 0 moveto show grestore} BD\n"+\
"/CCS {gsave translate newpath 0 0 moveto dup false charpath pathbbox\n"+\
" 3 -1 roll sub 2 div neg 3 1 roll sub 2 div exch moveto\n"+\
" show grestore} BD\n"+\
"\n"+\
"% Simple Path Construction:\n"+\
"/N {newpath} BD\n"+\
"/C {closepath} BD\n"+\
"/M {moveto} BD\n"+\
"/R {rmoveto} BD\n"+\
"/L {lineto} BD\n"+\
"/RL {rlineto} BD\n"+\
"\n"+\
"% Not So Simple Path Construction:\n"+\
"/RECT {/height exch D /width exch D /yo exch D /xo exch D\n"+\
" xo yo M width 0 RL 0 height RL width neg 0 RL C} BD\n"+\
"/o {N 2 copy 5 0 360 arc 2 0 360 arcn F} BD\n"+\
"\n"+\
"% Simple Paint Operations:\n"+\
"/ST {[] 0 setdash stroke} BD\n"+\
"/DST {[6 4] 3 setdash stroke} BD\n"+\
"/F {fill} BD\n"+\
"\n"+\
"% Default Font:\n"+\
"/Courier findfont 12 scalefont setfont\n"+\
"\n"+\
"%%EndProlog\n\n"

mapping ps_pages;

static void
create()
{
    ps_pages=([ ]);
}

/*
 * Function name: quote_string_ps
 * Description:   quotes parantheses in str
 * Arguments:     str - string to search
 * Returns:       
 */
string
quote_string_ps(string str)
{
    string *parts;

    if (!strlen(str))
	return "";
    
    parts=explode(str, "(");
    if (sizeof(parts)>1)
	str=implode(parts, "\\(");
    else
	str=parts[0];

    parts=explode(str, ")");
    if (sizeof(parts)>1)
	return implode(parts, "\\)");

    return parts[0];
}

/*
 * Function name: object_desc
 * Description:   Give a name for an object, depending on object type:
 *                room: name of room file
 *                living: name
 *                other object: path
 * Arguments:     object
 * Returns:       name of object
 */
static string
object_desc(object obj)
{
    string str;
    
    if (str=obj->query_name())
	return capitalize(str);

    if (str=(obj->query_room_file()))
	return str;

    return object_name(obj);
}

/*
 * Function name: open_ps
 * Description:   Open new file fname (remove if existant) and
 *                insert ps-header.
 * Arguments:     fname - name of file to open.
 */
void
open_ps(string fname)
{
    remove_file(fname);

    write_file(fname,
	       "%!PS-Adobe-3.0\n"+
	       "%%Pages: (atend)\n"+
	       "%%Title: "+quote_string_ps(fname)+"\n"+
	       "%%Creator: "+
	       quote_string_ps(object_desc(previous_object()))+"\n"+
	       "%%EndComments\n"+PS_PROLOG, 0);
    ps_pages[fname]=0;
}

/*
 * Function name: close_ps
 * Description:   Append trailer to file fname.
 * Arguments:     fname - name of file to close.
 * Returns:       Total number of pages written.
 */
int
close_ps(string fname)
{
    int total;
    total=ps_pages[fname];
    
    if (total)
	write_file(fname, "restore showpage\n", 0);
    
    write_file(fname,
	       "%%Trailer\n"+
	       "%%Pages: "+total+"\n"+
	       "%%EOF\n", 0);
    ps_pages[fname]=0;
    return total;
}

/*
 * Function name: newpage_ps
 * Description:   open a new page in file fname
 * Arguments:     fname - file to apply to
 * Returns:       the ordinal number of the page (1 for the first page)
 */
int
newpage_ps(string fname)
{
    int total;
    total=ps_pages[fname]+1;    

    if (total > 1)
	write_file(fname,
		   "restore showpage\n", 0);
    
    write_file(fname,
	       "%%Page: "+total+" "+total+"\n"+
	       "save\n", 0);
    return ps_pages[fname]=total;
}

/*
 * Function name: new_ranged_page_ps
 * Description:   open a new page in file fname
 *                showing a given range of coordinates
 * Arguments:     fname - file to apply to
 *                xo, yo, width, height - window in world coordinates.
 *                  This window will be centered on the page and scaled
 *                  with constant aspect ratio, possibly rotated,
 *                  for best fit.
 * Returns:       the ordinal number of the page (1 for the first page)
 */
int
new_ranged_page_ps(string fname, int xo, int yo, int width, int height)
{
    int total;
    total=newpage_ps(fname);

    write_file(fname,
	       xo+" "+yo+" "+width+" "+height+" RANGE\n", 0);

    return total;
}

/*
 * Function name: write_at_ps
 * Description:   write a string at a certain position
 *                on the current page
 * Arguments:     fname - file to write to
 *                x, y - position
 *                str - the string to write
 */
void
write_at_ps(string fname, int x, int y, string str)
{
    write_file(fname, "("+quote_string_ps(str)+") "+x+" "+y+" S\n", 0);
}

/*
 * Function name: line_ps
 * Description:   draw a line
 * Arguments:     fname - file to write to
 *                x1, y1, x2, y2 - position of start and end points
 */
void
line_ps(string fname, int x1, int y1, int x2, int y2)
{
    write_file(fname, "N "+x1+" "+y1+" M "+x2+" "+y2+" L ST\n", 0);
}

/*
 * Function name: rectangle_ps
 * Description:   draw boundary of a rectangle
 * Arguments:     fname - file to write to
 *                x1, y1, x2, y2 - corners of the rectangle
 */
void
rectangle_ps(string fname, int x, int y, int width, int height)
{
    write_file(fname, "N "+x+" "+y+" "+width+" "+height+" RECT ST\n", 0);
}

/*
 * Function name: named_box_ps
 * Description:   
 * Arguments:     
 * Returns:       
 */
void
named_mark_ps(string fname, int x, int y, string name)
{
    write_file(fname,
	       x+" "+y+" o\n"+
	       "("+quote_string_ps(name)+") "+x+" "+ (y+8) +" CS\n", 0);
}

/*
 * Function name: named_box_ps
 * Description:   
 * Arguments:     
 * Returns:       
 */
void
named_box_ps(string fname, int xo, int yo, int width, int height, string name)
{
    write_file(fname,
	       "N "+xo+" "+yo+" "+width+" "+height+" RECT ST\n"+
	       "("+quote_string_ps(name)+") "+
	       (xo+width/2)+" "+(yo+height/2)+
	       " CCS\n", 0);
}

/*
 * Function name: object_mark_ps
 * Description:   
 * Arguments:     
 * Returns:       
 */
void
object_mark_ps(string fname, object obj)
{
    int *co;

    named_mark_ps(fname,
		  (co=obj->query_coordinates())[0],
		  co[1], object_desc(obj));
}

/*
 * Function name: object_block_box_ps
 * Description:   
 * Arguments:     
 * Returns:       
 */
void
object_block_box_ps(string fname, object obj)
{
    int *co;
    
    named_box_ps(fname,
		 (co=obj->query_coordinates())[0], co[1],
		 (co=obj->query_range())[0], co[1],
		 object_desc(obj));
}

/*
 * Function name: room_snapshot_ps
 * Description:   Produces a postscript snapshot of the room.
 * Arguments:     fname - name of postscript file to append to;
 *                where - room to paint.
 */
void
room_snapshot_ps(string fname, object where)
{
    int *co, *range, i, sz;
    object *objs;

    co=where->query_coordinates();
    range=where->query_room_size(); 
    new_ranged_page_ps(fname, co[0], co[1], range[0], range[1]);

    write_file(fname,
	      "N "+co[0]+" "+co[1]+" "+range[0]+" "+range[1]+
	      " RECT DST\n", 0);
    
    if (sz=sizeof(objs=map_indices(where->query_event(E_BLOCK))))
	for (i=0; i<sz; i++)
	    object_block_box_ps(fname, objs[i]);

    if (sz=sizeof(objs=I(where, 2)))
	for (i=0; i<sz; i++)
	    object_mark_ps(fname, objs[i]);
}
