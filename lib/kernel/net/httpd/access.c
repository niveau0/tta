#include <config.h>

string password;

nomask int
check_read(string file, string user, string pw)
{
    restore_object(PLAYER_FILE(user));
    if (!password)
	return 0;
    return (crypt(pw, password) == password);
}

nomask int
check_write(string file, string user, string pw)
{
    restore_object(PLAYER_FILE(user));
    if (!password)
	return 0;
    return (crypt(pw, password) == password);
}
