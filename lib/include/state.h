#ifndef STATE_H
#define STATE_H

/*
 * the bit definitions for LIVE_STATE property
 */
#define SLEEPING     1
#define MEDITATING   2
#define PARALYSED    4
#define UNCONSCIOUS  8
#define COMBAT       16
#define CONCENTRATE  32
#define WALKING      64
#define SEARCHING    128
#define RESTING      256
#define HIDING       512
#define GHOST        1024
#define ON_GROUND    2048
#define MOUNT        4096
#define WORKING      8192

#define ALL_STATE    16383

#endif
