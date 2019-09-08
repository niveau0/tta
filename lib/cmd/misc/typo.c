/*
 * Function name: typo
 * Description:   give a typo information
 * Arguments:     str - info string
 *                tp - acting living
 * Returns:       1
 */
nomask int
typo(string str, object tp)
{
    if (!str)
    {
	tp->catch_tell("Usage: typo <text>\n" +
		       "       Please describe the mistyping.\n");
	return -1;
    }
    NEWS_MASTER->post_log(BS(str, 75, 0),
			  E(tp)->query_room_file(), "typos", tp);
    tp->catch_tell("Thanks for your help.\n");
    return 1;
}
