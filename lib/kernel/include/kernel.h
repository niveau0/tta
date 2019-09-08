# define _LOG_CALLOUT    /* create callout list */
# define _LOG_ECHO
# define _LOG_EMOTE
# define _LOG_ENTER
# define _LOG_EXEC
# define _LOG_FORCE
# define _LOG_KILLS
# define _LOG_LEVEL
# define _LOG_POSSESS
# define _LOG_SHUTDOWN
 
# define _LOG_SIZE	 50000
# define _FILE_CHUNK      5000
# define _TAIL_CHUNK      2000
# define _TAIL_LINES        30
# define _CAT_LINES         40
# define _SWAPCHUNK        500 /* how many objects to create before swapout */

# define _EDITOR    "/kernel/sys/editor"
# define _INDENT    "/kernel/sys/indent"
# define _VBFC      "/kernel/lib/vbfc" 
# define _LS(s)     call_other("/kernel/lib/ls", "ls", s);

# define _EDITORLOG ".editors"

# define PRIVILEGED() (previous_program() == _AUTO)
# define BADARG(n, f) error("Bad argument " + (n) + " to function " + #f)