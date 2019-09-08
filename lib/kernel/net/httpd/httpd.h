#define HTTPD_PATH "/kernel/net/httpd/"

#define WWWBASE    "/www"
#define CGIBIN     "/www/cgi-bin"
#define ICONS      (HTTPD_PATH + "icons/")
#define INDEXFILE  "index"
#define AUTHFILE   ".htaccess"
#define SERVERNAME "HTTPD/0.4 DGD/LPC"
#define BLOCK_SIZE 1024

#define FILE       0 /* transfer type */
#define STRING     1 /* transfer type */

#define CHECK_HTTPREAD(f, u, p) \
call_other(HTTPD_PATH + "access", "check_read", f, u, p)

#define CHECK_HTTPWRITE(f, u, p) \
call_other(HTTPD_PATH + "access", "check_write", f, u, p)
