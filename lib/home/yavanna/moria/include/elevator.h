#ifndef elevator_master
#define elevator_master
/* elevator move directions */
#define DIRECTION_UP 1
#define DIRECTION_DOWN -1
#define ELEVATOR_HALTED 0
/* Positions in Data-mapping of an elevator in elevator_data */
#define MOVE_SPEED 0
#define MOVE_DIRECTION 1
#define ELEVATION 2
#define WINCH_OBJ 3
#define OUTSIDE_ROOMS 4
/* */
#define INSIDE_DOORS 5
#define OUTSIDE_DOORS 6
/* Positions in Data-maping of an elevator in elevator_doors */
#define DOORS_INNER 0
#define DOORS_OUTER 1

/* Property Values*/
#define IS_ELEVATOR 7015

#endif
