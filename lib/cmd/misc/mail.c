/*
 * Function name: mail
 * Description:   start reading/writing mail
 * Arguments:     str - optional receiver name
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
mail(string str, object tp)
{
    string roomfile;

    roomfile = E(tp)->query_room_file();
    if (_ACCESS_LEVEL(geteuid(tp)) < 1 &&
	(!roomfile || strlen(roomfile) <= strlen(OOCAREA) ||
	 roomfile[..strlen(OOCAREA)-1] != OOCAREA))
    {
	tp->catch_tell("Only possible in out-of-character areas.\n");
	return -1;
    }
    clone_object(MAIL_MASTER)->read_mail(str);
    return 1;
}
