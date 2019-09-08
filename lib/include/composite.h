#ifndef COMPOSITE_H
#define COMPOSITE_H

#define COMPOSITE_FILE "/system/composite"

#define DIRDESC_TO_INT(s) \
call_other(COMPOSITE_FILE, "dirdesc_to_integer", s)

#define COORD_SUFFIX(s, d) \
call_other(COMPOSITE_FILE, "coord_suffix", s, d)

#define COMPOSITE_WORDS(wl)\
call_other(COMPOSITE_FILE, "composite_words", wl)

#define COMPOSITE(x, p) \
call_other(COMPOSITE_FILE, "composite", x, p)
    
#define COMPOSITE_UNSEEN(x, p) \
call_other(COMPOSITE_FILE, "composite_unseen", x, p)

#define COMPOSITE_ARRAY(x) \
call_other(COMPOSITE_FILE, "composite_array", x)

#define COMPOSITE_ARRAY_ALL(x) \
call_other(COMPOSITE_FILE, "composite_array", x, 1)

#define COMPOSITE_ROOM_CONTENTS(r, l) \
call_other(COMPOSITE_FILE, "composite_room_contents", r, l)
    
#endif
