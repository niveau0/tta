/*
 * Function name: news
 * Description:   read news
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
news(string str, object tp)
{
    if (_ACCESS_LEVEL(geteuid(tp)))
	clone_object(NEWS_MASTER)->read_news(tp);
    else
	more(LOGIN_FILE_NEWS, tp);
    return 1;
}
