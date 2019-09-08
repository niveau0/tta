/*
 * Function name: from
 * Description:   list current mail
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
from(string str, object tp)
{
    MAIL_MASTER->from_cmd(tp);
    return 1;
}
