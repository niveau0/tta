#ifndef ROOM_H
#define ROOM_H

/* exit data indices */
#define EXIT_DIR    0
#define EXIT_OFFX   1
#define EXIT_OFFY   2
#define EXIT_WIDTH  3
#define EXIT_HEIGHT 4
#define EXIT_STATUS 5
#define EXIT_OBJ    6

#define ROOM_EDGE_N    0
#define ROOM_EDGE_E    1
#define ROOM_EDGE_S    2
#define ROOM_EDGE_W    3
#define ROOM_EDGE_ALL -1

/* exit status values    |  walk   |  look   |  hear   |
 *                       | through | through | through |
 *------------------------------------------------------*/
#define EXIT_OPEN   0  /*|   yes   |   yes   |   yes   |*/
#define EXIT_INVIS  1  /*|   yes   |   no    |   yes   |*/
#define EXIT_CLOSED 2  /*|   no    |   no    |   yes   |*/

#endif
