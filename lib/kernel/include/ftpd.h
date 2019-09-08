/* ftpd.h */
#ifndef __FTPD_H
#define __FTPD_H

#define FTP_LOG      "FTP"
#define FTPD_VERSION "1.0"
#define FTPDATACONN  "/kernel/net/ftpdata"
#define FTPCLIENT    "/kernel/net/ftpd"
#define FTPPORTOBJ   "/kernel/net/port/ftp"

#define MAX_FTP_SESSIONS 10
#define MAX_UNIQUE_TRIES 10

#define BINARY     'b'
#define ASCII      'a'

#define STRING     's'
#define FILE       'f'

#define NO_APPEND  -1
#define APPEND      1

/* Set this to be the size of each outgoing block of data during
 * file transfers.  This hasn't been tested with any value other
 * than 1024, so its generally good to leave this alone.
 */
#define BLOCK_SIZE 1024

#define LOGGED \
if (!logged || !name) \
{ send_message("533 Log in with USER first.\n"); break; }

#define MONTHS ({ "Jan", "Feb", "Mar", "Apr", "May", "Jun", \
                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" })

#define SECS_IN_YEAR (365 * 24 * 60 * 60)

#endif /* __FTPD_H */
