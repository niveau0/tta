/* **************************************************************************
 * blocksite - banish sites
 */
nomask int
blocksite(string str)
{
    CHECK_LEVEL(_ADMIN);
    
    return _SYSTEMD->block_site(str);
}
