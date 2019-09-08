#define INIT_PHASE    1
#define INIT_PHASE2   2
#define DICE_PHASE    3
#define BUILD_PHASE   4
#define THIEF_PHASE   5
#define KNIGHT_PHASE  6 /* knight played before roll */
#define DISCARD_PHASE 7

#define FOREST 1
#define ORE    2
#define WOOL   3
#define STONE  4
#define WHEAT  5
#define DESERT 6
/* development types */
#define KNIGHT       7
#define MONOPOL      8
#define ROADBUILD    9
#define INVENTION    10
/* cards that still need to be discarded */
#define DISCARD      11

#define DEVTYPES ({ KNIGHT, MONOPOL, ROADBUILD, INVENTION })
#define RESDESC \
({ 0, "forest", "ore", "wool", "stone", "wheat", "desert", \
       "knight", "monopol", "roadbuild", "invention" })
    
#define COLORS \
({ "red", "blue", "white", "green", "cyan", "orange", "violet", "yellow" });

#define COL_RGB \
([\
 "red"    : "#ff6060",\
 "blue"   : "#6060ff",\
 "white"  : "#e0e0e0",\
 "green"  : "#60ff60",\
 "cyan"   : "#60ffd0",\
 "orange" : "#f0a030",\
 "violet" : "#b070ff",\
 "yellow" : "#f0e060",\
    ])

#define XHEX   9
#define YHEX   5
#define LINE \
"                                                                           \n"
#define HEX \
"  >---<  "+\
" /     \\ "+\
"<       >"+\
" \\     / "+\
"  >---<  "
