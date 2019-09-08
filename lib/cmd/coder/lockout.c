/* **************************************************************************
 * lockout - lock new player creation from site
 */
nomask int
lockout(string str)
{
    CHECK_LEVEL(_ADMIN);

    return _SYSTEMD->lockout_site(str);
}
