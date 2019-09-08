/*
 * Function name: idea
 * Description:   write down an idea
 * Arguments:     str - info string
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
idea(string str, object tp)
{
    if (!str)
    {
	tp->catch_tell("Please describe your idea.\n");
	return -1;
    }
    NEWS_MASTER->post_log(BS(str, 75, 0),
			  E(tp)->query_room_file(), "idea", tp);
    tp->catch_tell("Thanks for your help.\n");
    return 1;
}
