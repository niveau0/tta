#include <access.h>
#include <event.h>
#include <filepath.h>
#include <language.h>
#include <limits.h>
#include <macros.h>
#include <move.h>
#include <mudtime.h>
#include <properties.h>
#include <room.h>
#include <weather.h>

#define CHANGED 1
#define SAVED   2

#define STORED_ENV "_creator_env"

#define CLIMATE_CHOICE      1
#define COORDINATES_CHOICE  2
#define EXIT_CHOICE         3
#define HEIGHT_CHOICE       4
#define ITEM_CHOICE         5
#define LIGHT_CHOICE        6
#define LONG_CHOICE         7
#define OFFER_CHOICE        8
#define PARENT_CHOICE       9
#define PATH_CHOICE        10
#define SHORT_CHOICE       11
#define SIZE_CHOICE        12
#define TYPE_CHOICE        13
#define BORDER_CHOICE      14

#define CLIMATES \
({ \
       "medium", CLIMATE_MEDIUM, \
       "medium humid", CLIMATE_MEDIUM_HUMID, \
       "medium arid", CLIMATE_MEDIUM_ARID, \
       "hot", CLIMATE_HOT, \
       "hot humid", CLIMATE_HOT_HUMID, \
       "hot arid", CLIMATE_HOT_ARID, \
       "cold", CLIMATE_COLD, \
       "cold humid", CLIMATE_COLD_HUMID, \
       "cold arid", CLIMATE_COLD_ARID, \
       "polar", CLIMATE_POLAR \
       })

#define ROOM_TYPES \
({ \
       "inside", TYPE_INSIDE, \
       "underground", TYPE_UNDER_GROUND, \
       "underwater", TYPE_UNDER_WATER, \
       "ocean", TYPE_OCEAN, \
       "coast", TYPE_COAST, \
       "river", TYPE_RIVER, \
       "lake", TYPE_LAKE, \
       "hills", TYPE_HILLS, \
       "mountains", TYPE_MOUNTAINS, \
       "swamp", TYPE_SWAMP, \
       "waste", TYPE_WASTE, \
       "short grass", TYPE_SHORT_GRASS, \
       "long grass", TYPE_LONG_GRASS, \
       "light forest", TYPE_LIGHT_FOREST, \
       "dense forest", TYPE_DENSE_FOREST, \
       "jungle", TYPE_JUNGLE, \
       "village", TYPE_VILLAGE, \
       "town", TYPE_TOWN, \
       "shop", TYPE_SHOP, \
       })

#define HELP_CLIMATE \
"This edit-function toggles the climate of your room. The climates find\n"+\
"their equivalent on the world of the 20th Century: \n"+\
"     medium - Middle Europe, ie. Northern France, Belgium, Germany,\n"+\
"              Middle China, Japan\n"+\
"        hot - Southern Europe, Southern North America\n"+\
"  hot humid - Middle America, South America, Middle Africa\n"+\
"   hot arid - North Africa (deserts)\n"+\
"       cold - Northern Europe, ie. Skandinavia, Canada\n"+\
" cold humid - Regions in the Antarktis, Arktis\n"+\
"  cold arid - Regions in the Antarktis, Arktis\n\n"+\
"For a guideline concerning climates of certain regions in 'The Third Age'\n"+\
"please refer to the 'help climate' page.\n\n"

#define HELP_COORDINATES \
"A rooms coordinates are the coordinates of the lower southwestern corner\n"+\
"of the room.\n"+\
"Example:        *------*\n"+\
"   Up          /|     /|\n"+\
"    ^ _.North / |    / |\n"+\
"    | /|     *--+---*  |\n"+\
"    |/       |  |   |  |\n"+\
" ---*--->    |  *---+--*\n"+\
"   /| East   | /    | /\n"+\
"  / |        |/     |/\n"+\
"             *------*\n"+\
"             ^\n"+\
"             |\n"+\
"        this point\n\n"

#define HELP_EXIT \
"An exit is a rectangular form within the wall of a room leading to a\n"+\
"room which lies directly at this wall.\n"+\
"Example:        *------*------*\n"+\
"   Up          /| ___ /|     /|\n"+\
"    ^ _.North / |/__//,|    / |\n"+\
"    | /|     *------*------*  |\n"+\
"    |/       |  |+-+||||   |  |\n"+\
" ---*--->    |  *|-|||/*---|--*\n"+\
"   /| East   | / | ||'/    | /\n"+\
"  / |        |/  +-+|/     |/\n"+\
"             *------*------*\n"+\
"You have to specify its direction in words or a single letter ...\n"+\
"(n)orth, (s)outh, (u)p, (d)own, (e)ast, (w)est\n"+\
"(You can also enter 'spread', in order to create exits in all rooms where\n"+\
"the current exits of this room lead to.)"+\
"... and the room it should lead to. This exit room is named with either\n"+\
"its absolute pathname or the one relative to the edited room. Behind the\n"+\
"filepath you can add the option 'full' separated with a comma. This will\n"+\
"cause the room creator to set an exit with the minimum offsets and\n"+\
"maximum sizes towards the exitroom.\n"+\
"Knowing the exits direction the roomcreator is still in need of its\n"+\
"position. You specify this data with an offset of, depending on the\n"+\
"direction, a combination of an x-, y- or z-offset. This offset is always\n"+\
"beginning at the southern, western or lower border of the room.\n"+\
"The room creator delivers the minimum and maximum offsets for the chosen\n"+\
"exitroom.\n"+\
"If the up to now specified exit already exists you have the possibility\n"+\
"to remove it by typing 'remove'. (Easy, isn't it? ;)\n"+\
"Now the size of the exit is questioned. Again depending on its direction\n"+\
"you enter the width and the height. Notice that this two length do not\n"+\
"necessarily mean horizontal and vertical. In case of an exit within the\n"+\
"floor or the ceiling, the width means east-west length and height the\n"+\
"north-south length. To support you, the room creator shows the maximum\n"+\
"size of the exit, considering the offsets and the exitroom's size.\n"+\
"As the maximum height of a room is a pretty large number, you have the\n"+\
"option to enter 'outside' for the exitheight. That will cause\n"+\
"a height of DEFAULT_ROOM_HEIGHT. (="+DEFAULT_ROOM_HEIGHT+" right now)\n\n"

#define HELP_ITEM \
"Roomitems are a bit different from items which are cloned and put into a\n"+\
"room. These room items are not shown in the rooms description and their\n"+\
"main purpose is to allow further examinations of things described in the\n"+\
"roomdescription.\n"+\
"In order to edit a roomitem you have to refer to its id-number. (use\n"+\
"'view item' to get these numbers) For defining a new item you can either\n"+\
"enter an id of your choice or type 'new' to get a free id-number automa-\n"+\
"tically.\n"+\
"Then you enter a list of the singular names the item should be examinable\n"+\
"under. Same procedure with the plural names follows.\n"+\
"Afterwards you enter the description that shall be passed to the examiner\n"+\
"of the item.\n"+\
"Additionally you have the possibility to enter coordinates\n"+\
"for the item. They are absolute coordinates, which allow a living to\n"+\
"choose this item also as a goal it can travel to.\n\n"

#define HELP_HEIGHT \
"The height of a room is its vertical length, to say it mathematically.\n"+\
"The length is measured in inches and the current maximum length for rooms\n"+\
"that are outside can be entered by typing 'default'.\n"+\
"The current value for this maximum height is: "+DEFAULT_ROOM_HEIGHT+"\n\n"

#define HELP_LIGHT \
"'edit light' sets the rooms own ambient light. This value does not\n"+\
"include the daylight or the light of burning objects in the room.\n"+\
"The value is measured in 'how many inches far can I see with this light'.\n"+\
"The normal daylight is set to: "+DAYLIGHT+"\n"+\
"The normal nightlight is set to: "+NIGHTLIGHT+"\n\n"

#define HELP_LONG \
"With this function you start a line oriented editor which can be used to\n"+\
"define a closer description of the rooms interior. This description will\n"+\
"be passed to a living that uses the 'look' command within this room. Of\n"+\
"course this description can only be seen, if there is either enough light\n"+\
"in the room or the living has the ability to see in the dark.\n"+\
"Upon further information about the available commands of the editor type\n"+\
"'~?' for its builtin help-page.\n"+\
"(Notice, every new room has a default long-description.)\n\n"

#define HELP_OFFER \
"If this room is created/edited with the 'shop'-option you can set a list\n"+\
"of merchandises which can be sold here. Of course, you need something\n"+\
"working as shopkeeper, but that's a different story.\n"+\
"After 'edit offer' you enter:\n"+\
"the absolute filepath of the ware you want to add to the offerlist,\n"+\
"the amount of wares the shop shall start with,\n"+\
"the maximum amount it can hold of this ware and\n"+\
"the timespan it needs to produce one unit of this ware [in seconds]\n"+\
"... and all this separated by commata.\n"+\
"If you wish to remove an offer from the offerlist, enter the absolute\n"+\
"filepath of the ware to remove and after a comma the word 'remove'.\n"+\
"Notice: The '.c' at the filename's end must NOT be added.\n\n"

#define HELP_PARENT \
"No help available right now. Sorry.\n\n"

#define HELP_PATH \
"This function changes the (absolute) filepath for the edited room.\n"+\
"The path has to begin with '/d/<Domain>/' or '/home/<yourname>/'.\n\n"

#define HELP_SHORT \
"The shortdescription of a room shall deliver a rough overview of the\n"+\
"room. Therefore it shall be not longer than one line. Imagine, it is what\n"+\
"you see when you stand in this place in the middle of the night.\n"+\
"Usually the description starts with a capital letter and shall describe\n"+\
"what you see only, not what you know.\n"+\
"Example: On a small road\n\n"

#define HELP_SIZE \
"The 'edit size' command specifies the room's size in north-south and\n"+\
"in east-west direction. The height of the room can be specified by using\n"+\
"the 'edit height' command.\n"+\
"By the way, a room with the coordinate 0,0,0 and the size 5,4 will have\n"+\
"its lower north-eastern corner at the coordinate 4,3,0.\n\n"

#define HELP_BORDER \
"The latest improvements in roomconstruction delivered the possibility to\n"+\
"add a level for the northern, eastern, southern and western border of a\n"+\
"room. This borderlevel is measured in inches and relative to the absolute\n"+\
"z-coordinate of the room. This way a room can contain slopes.\n\n"

#define HELP_TYPE \
"Every room can have several types. That means different abilities or\n"+\
"strength is needed to travel in there. The roomtypes can also influence\n"+\
"the strength of the daylight or cause less pleasant things to happen.\n"+\
"Who knows which devlish ideas still might come to the coders of this MUD.\n"+\
"Well, to say it briefly, choose the best fitting roomtypes for the edited\n"+\
"room from the list below and enter them one by one. To remove an already\n"+\
"chosen type from the list just enter it once more and it will be removed.\n\n"

