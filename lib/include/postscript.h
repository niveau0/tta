#ifndef POSTSCRIPT_H
#define POSTSCRIPT_H

#define PSTOOLS_FILE "/system/postscript"

/*
 * Tools for generating arbitrary postscript output.
 * Recommended order for low level painting:
 *   PS_OPEN [ PS_NEWPAGE [ paint-ops ]+ ]+ PS_CLOSE
 * See sections below for paint-ops.
 */

#define PS_OPEN(fname) call_other(PSTOOLS_FILE,"open_ps",fname)
#define PS_CLOSE(fname) call_other(PSTOOLS_FILE,"close_ps",fname)
#define PS_NEWPAGE(fname) call_other(PSTOOLS_FILE,"newpage_ps",fname)
#define PS_NEW_RANGED_PAGE(fname,xo,yo,width,height) \
call_other(PSTOOLS_FILE,"new_ranged_page_ps",fname,xo,yo,width,height)

/* Paint-ops are: */
#define PS_WRITE_AT(fname,x,y,str) \
call_other(PSTOOLS_FILE,"write_at_ps",fname,x,y,str)

#define PS_LINE(fname,x1,y1,x2,y2) \
call_other(PSTOOLS_FILE,"line_ps",fname,x1,y1,x2,y2)

#define PS_RECT(fname,x,y,width,height) \
call_other(PSTOOLS_FILE,"rectangle_ps",fname,x,y,width,height)

#define PS_NAMED_MARK(fname,x,y,str) \
call_other(PSTOOLS_FILE,"named_mark_ps",fname,x,y, str)

#define PS_NAMED_BOX(fname,x,y,width,height,str) \
call_other(PSTOOLS_FILE,"named_box_ps",fname,x,y,width,height, str)

#define PS_OBJECT_MARK(fname,obj) \
call_other(PSTOOLS_FILE,"object_mark_ps",fname,obj)

#define PS_OBJECT_BOX(fname,obj) \
call_other(PSTOOLS_FILE,"object_block_box_ps",fname,obj)

/* Higher level painting: */
#define PS_ROOM_SNAPSHOT(fname, where) \
call_other(PSTOOLS_FILE,"room_snapshot_ps",fname,where)

#endif
