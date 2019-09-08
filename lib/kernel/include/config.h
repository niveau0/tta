/*
 * Contains world specific information.
 */

# ifndef CONFIG_DEF
# define CONFIG_DEF

/*
 * MUDLIB_VERSION - simply the version of the mudlib
 * MUD_NAME       - name of the mud (to be printed on welcome screens)
 * MUD_ABBREV     - abbreviationname of the mudname (e.g. for intermud line)
 * ADMIN_EMAIL    - email of the administration
 * REGULAR_REBOOT - The hour when reboot occurs (if negative: no reboot)
 */
# define MUDLIB_VERSION "TTA-0.9"
# define MUD_NAME       "The Third Age"
# define MUD_ABBREV     "tta"
# define ADMIN_EMAIL    "niveau0@t-online.de"
# define REGULAR_REBOOT -1

/*
 * GARBAGE_INTERVAL - time interval for garbage collection 
 * GARBAGE_OBJECTS  - processed objects each interval
 * GARBAGE_LOG      - Logfile name 
 */
# define GARBAGE_INTERVAL 3600 
# define GARBAGE_OBJECTS  50   
# define GARBAGE_LOG      "GARBAGE"

/*
 * PLAYER_DIR  - directory for the player
 * PLAYER_FILE - filename for saving the player object
 * PLAYER_BASE - filename for the basic player code
 * RACE_FILE   - macro to finding the correct file for a race
 */
# define PLAYER_DIR(name)  ("/players/" + extract(name,0,0) + "/" + name)
# define PLAYER_FILE(name) (PLAYER_DIR(name) + "/" + name)
# define PLAYER_BASE       "/base/player"
# define RACE_FILE(race)   ("/config/race/"+race+"/" + race)

/*
 * CREATE_PLAYER         - new player editor
 * CREATE_ROOM           - room editor
 * CREATE_SHOP           - shop editor
 * LOGIN_ROOM            - temporal environment of player while login
 * LOGIN_FILE_WELCOME    - path of startup-screens
 * LOGIN_FILE_NEWS       - news to show at login
 */
# define CREATE_PLAYER         "/system/creator/player"
# define CREATE_ROOM           "/system/creator/room"
# define CREATE_SHOP           "/system/creator/shop"
# define LOGIN_GENERATE_PLAYER "/config/login/generation"
# define LOGIN_ROOM            "/config/login/login_room"
# define LOGIN_FILE_WELCOME    "/config/login/WELCOME"
# define LOGIN_APPLICATION_DIR "/players/application"
# define LOGIN_FILE_NEWS       "/doc/NEWS"
# define LOGIN_CODER_NEWS      "/doc/CODERNEWS"

/*
 * IMPROVE_TIME  - stat improve time (sort of level raise)
 * CONSUME_TIME  - hunger and thirst speed
 * AUTOSAVE_TIME - how often should players be saved
 * MAX_IDLE_TIME - maximum time you are guaranteed to stay idle before being
 *                 logged out. The amount is counted in seconds.
 * MAXFILES      - maximum number of files that can be copied/moved/removed
 *                 with one command. Protects against errors like 'rm *'
 */
# define IMPROVE_TIME  345600 /* 4 real days */
# define CONSUME_TIME  600
# define AUTOSAVE_TIME 1800
# define MAX_IDLE_TIME 1800
# define MAXFILES      20

/*
 * WEATHER_MASTER - handles weather
 * MAIL_MASTER    - mail reader
 * NEWS_MASTER    - news reader
 * LEARN_OB       - the global learn object for skills
 * DEAD_SOUL      - the object used for dead players
 * EDITOR         - the global editor object (mails, news, etc.)
 * EDIT(f, t)     - clone an editor, f - return function, t - initial text
 */
# define WEATHER_MASTER "/system/weathermaster"
# define MAIL_MASTER    "/system/mailmaster"
# define NEWS_MASTER    "/system/newsmaster"
# define LEARN_OB       "/system/learn"
# define DEAD_SOUL      "/config/race/ghost/ghost"
# define EDITOR         "/system/edit"
# define EDIT(f, t)     clone_object(EDITOR)->edit(f, t)

/*
 * CODER_SOUL    - cmdsoul of the coders
 * MISC_SOUL     - miscellanous commands
 * EMOTION_SOUL  - emotional commands
 * DEFAULT_SOULS - array of cmdsouls that all mobiles should use
 */
# define CODER_SOUL    "/cmd/coder/coder"
# define MISC_SOUL     "/cmd/misc/misc"
# define EMOTION_SOUL  "/cmd/emotion/emotion"
# define DEFAULT_SOULS ({ EMOTION_SOUL, MISC_SOUL })

/*
 * OOCAREA             - out-of-character area directory
 * DEFAULT_START       - default starting room if nothing else is defined
 * DEFAULT_ROOM_HEIGHT - default height of outside rooms
 * DEFAULT_CODERS      - default setup of coders. mapping contents are:
 *                    ([ coder_name : ({ coder_level, coder_domain }) ])
 */
# define OOCAREA             "/d/Standard/room/ooc"
# define DEFAULT_START       "/d/Standard/room/ooc/general"
# define DEFAULT_ROOM_HEIGHT 100000000

/*
 * DEFAULT_WALK    - default walking message
 * DEFAULT_RUN     - default running message
 * DEFAULT_TELEIN  - default teleport arrive message
 * DEFAULT_TELEOUT - default teleport leave message
 */
# define DEFAULT_WALK 	 "walks"
# define DEFAULT_RUN 	 "runs"
# define DEFAULT_TELEIN  "arrives in a puff of smoke."
# define DEFAULT_TELEOUT "disappears in a puff of smoke."
    
# define MELEE_DISTANCE     36 /* minimum melee fighting distance */

# endif
