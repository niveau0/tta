/* **************************************************************************
 * editplayer - create a new player
 */
nomask int
editplayer(string arg)
{
    CHECK_LEVEL(_CREATOR);
    return clone_object(CREATE_PLAYER)->cr_create(arg);
}
